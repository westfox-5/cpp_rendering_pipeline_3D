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
            int num_objects_assigned = 0;

            // assign for each thread its work
            for (int thread_count = 0; thread_count < num_threads_; ++thread_count) {
                
                // if modulus > 0, give to the thread one more object 
                // so that load is equally distributed
                int actual_load = load_per_thread;
                if (num_remaining_objs > 0) {
                    ++actual_load;
                    --num_remaining_objs;
                }

                // extract the vector of objects to assign
                std::vector<Object> splitted(objects.begin() + num_objects_assigned, objects.begin() + (num_objects_assigned + actual_load));
                
                // increment the number of objects assigned so far
                num_objects_assigned += actual_load;

                // create the thread
                //std::thread th( [this, rasterizer, splitted] { this->_fun_scene_mode_(std::move(splitted), rasterizer); } );
                //workers.emplace_back( std::thread(&Scene::_fun_scene_mode_) );

                auto functor = 
                    [this]() ->void {this->_fun_scene_mode_();};

                std::thread t1(functor);
                workers[thread_count] = std::thread(functor);

            }

            for (auto &t: workers) { 
                t.join();
            }

        } 
    }



private:
    std::vector<Object> objects;
    std::mutex mtx;
    
    void _fun_scene_mode_() {/*std::vector<Object> objects){/*, Rasterizer<target_t>&& rasterizer) {*/
        mtx.lock();
        std::cout << "works?" << std::endl;
        mtx.unlock();
        //std::cout << objects.size() << std::endl;
        /*
        for (auto& o : objects) {
            o.render(*rasterizer,view_);
        }
        */
    }
};

};
#endif // SCENE_H
