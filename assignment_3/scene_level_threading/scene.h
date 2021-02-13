#ifndef SCENE_H
#define SCENE_H
#include<memory>
#include<utility>

#include<math.h>
#include<thread>
#include<mutex>

#include"rasterization.h"



#include<iostream>
namespace pipeline3D {


const std::array<float,16> Identity{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

/*** Defines where to apply multithreads */

/** Apply multithreading to the whole scene rendering: 
 * 
 *  divides the number of objects to render between the available threads.
 *  each thread renders all its assigned objects
*/
const static int _MULTITHREADING_MODE_SCENE_   = 0;

/** Apply multithreading to each triangle of each object:
 * 
 * 
*/
const static int _MULTITHREADING_MODE_TRIANGLES_  = 1;

template<class target_t>
class Scene
{
public:

    Scene(): view_(Identity), num_threads_(1), multithreading_mode_(_MULTITHREADING_MODE_SCENE_) {};
    std::array<float,16> view_;
    
    int num_threads_;

    int multithreading_mode_;

    class Object{
    public:

        Object(const Object&) = delete;
        Object(Object&&) = default;

        template<class Mesh, class Shader, class... Textures>
        Object(Mesh &&mesh, Shader&& shader, Textures&&... textures) :
            pimpl(std::make_unique<concrete_Object_impl<Mesh,Shader,Textures...>>(std::forward<Mesh>(mesh), std::forward<Shader>(shader), std::forward<Textures>(textures)...)), world_(Identity) {}

          void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view) {pimpl->render(rasterizer,view,world_);}

          std::array<float,16> world_;

    private:

        struct Object_impl {
          virtual ~Object_impl() {}
          virtual void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view, const std::array<float,16>& world)=0;
        };

        template<class Mesh, class Shader, class... Textures>
        class concrete_Object_impl : public Object_impl {
        public:
            concrete_Object_impl(Mesh &&mesh, Shader &&shader, Textures&&... textures ) :
                mesh_(std::forward<Mesh>(mesh)), shader_(std::forward<Shader>(shader)), textures_(std::forward<Textures>(textures)...) {}

            void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view, const std::array<float,16>& world) override {
                for(const auto& t : mesh_) {
                    auto v1=t[0];
                    auto v2=t[1];
                    auto v3=t[2];
                    
                    transform(world,v1);
                    transform(view,v1);
                    transform(world,v2);
                    transform(view,v2);
                    transform(world,v3);
                    transform(view,v3);
                    
                    rasterizer.render_vertices(v1,v2,v3, shader_);
                }
            }


        private:
            Mesh mesh_;
            Shader shader_;
            std::tuple<Textures...> textures_;
        };

        std::unique_ptr<Object_impl> pimpl;
    };

    void add_object(Object&& o) {objects.emplace_back(std::forward<Object>(o));}

    Object& operator[] (size_t i) {return objects[1];}
    size_t size() const {return objects.size();}
    auto begin() {return objects.begin();}
    auto end() {return objects.end();}

    void render(Rasterizer<target_t>& rasterizer) {

        if (_MULTITHREADING_MODE_SCENE_ == multithreading_mode_) {

            // calculate load of work per threads
            
            const int load_per_thread = (int)(std::floor(objects.size() / num_threads_));
            int num_remaining_objs = objects.size() % num_threads_;


            std::thread workers[num_threads_];
            int num_objects_assigned_so_far = 0;

            // assign for each thread its work
            for (int thread_count = 0; thread_count < num_threads_; ++thread_count) {
                
                // if modulus > 0, give to the thread one more object 
                // so that load is equally distributed
                int actual_load = load_per_thread;
                if (num_remaining_objs > 0) {
                    ++actual_load;
                    --num_remaining_objs;
                }
                

                //std::vector<Object> splitted (actual_load);
                //std::copy( objects.begin() + num_objects_assigned_so_far, objects.begin() + num_objects_assigned_so_far + actual_load , splitted.begin() );
                
/*
                std::vector<Object> splitted(
                    std::make_move_iterator(objects.begin() + num_objects_assigned),
                    std::make_move_iterator(objects.begin() + (num_objects_assigned + actual_load))
                );
*/
                // extract the vector of objects to assign
                //auto start_it = std::next(objects.begin(), num_objects_assigned);
                //auto end_it = std::next(objects.begin(), (num_objects_assigned + actual_load));
                //splitted.resize(actual_load);

                //std::copy(start_it, end_it, splitted.begin());

                // create the thread
                //std::thread th( [this, rasterizer, splitted] { this->_fun_scene_mode_(std::move(splitted), rasterizer); } );
                //workers.emplace_back( std::thread(&Scene::_fun_scene_mode_) );


                auto first = objects.begin() + num_objects_assigned_so_far;
                auto last = first + actual_load;

                if ( num_objects_assigned_so_far + actual_load > objects.size()) {
                    last = objects.end();
                }
                
                workers[thread_count] = std::thread([&]{
                    this->_fun_scene_mode_(first, last, rasterizer); } );

                // increment the number of objects assigned so far
                num_objects_assigned_so_far += actual_load;

                std::cout << "\t load assigned: "<<actual_load<< " - total so far: " << num_objects_assigned_so_far << std::endl;

            }

            for (auto &t: workers) { 
                t.join();
            }

        } 
    }



private:
    std::vector<Object> objects;

    //std::mutex mtx;
    
    template <class Iter>
    void _fun_scene_mode_(Iter first, Iter last, Rasterizer<target_t>& rasterizer) {
        
        //std::cout<<objects.size()<<std::endl;


        while ( first != last) {
            //mtx.lock();
            first->render(rasterizer, view_);
            //mtx.unlock();

            ++first;
        }

        std::thread::id this_id = std::this_thread::get_id();
        std::cout<< "Thread [" << this_id << "] end"<<std::endl; 
        
    }
};

};
#endif // SCENE_H
