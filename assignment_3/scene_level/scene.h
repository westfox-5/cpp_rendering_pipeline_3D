#ifndef SCENE_H
#define SCENE_H
#include<memory>
#include<utility>

#include<math.h>
#include<thread>

#include"rasterization.h"



#include<iostream>
namespace pipeline3D {


const std::array<float,16> Identity{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

template<class target_t>
class Scene
{
public:

    Scene(): view_(Identity), n_threads(1) {};
    std::array<float,16> view_;
    
    int n_threads;

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

        // LOAD BALANCING  PER THREAD
        
        const int load_per_thread = (int)(std::floor(objects.size() / n_threads));
        int num_remaining_objs = objects.size() % n_threads;
        int num_objects_assigned_so_far = 0;

        // WORKER CREATION

        std::vector<std::thread> workers(n_threads);

        for (int thread_count = 0; thread_count < n_threads; ++thread_count) {
            
            // if the modulus is > 0, objects is not divisible by n_threads
            // so we need to assign more objects to each thread.
            //
            // by assigning one more to each, we are sure to correctly balance
            // all worker's load
            int num_objects_to_assign = load_per_thread;
            if (num_remaining_objs > 0) {
                ++num_objects_to_assign;
                --num_remaining_objs;
            }
            
            auto first = objects.begin() + num_objects_assigned_so_far;
            auto last = first + num_objects_to_assign; // size from first to last is num_objects_to_assign

            // if calculus overflows objects, stop at last object
            if ( num_objects_assigned_so_far + num_objects_to_assign > objects.size()) {
                last = objects.end();
            }
            
            // increment the number of objects assigned so far
            num_objects_assigned_so_far += num_objects_to_assign;

            // create thread with the worker function
            std::thread thread([&]{ this->render_chunck(first, last, rasterizer); } );

            workers.push_back( std::move(thread) );

            #ifndef DEBUG
                std::cout << "\t load assigned: "<<num_objects_to_assign<< " - total so far: " << num_objects_assigned_so_far << std::endl << std::flush;
            #endif
        }

        for (std::thread &t: workers) { 
            if (t.joinable()) {
                t.join();
            }
        }

    }



private:
    std::vector<Object> objects;

    // Worker function to render a chunck of objects, from first to last, assigned by the balancer
    template <class Iterator>
    void render_chunck(Iterator first, Iterator last, Rasterizer<target_t>& rasterizer) {
        
        while ( first != last) {
            first->render(rasterizer, view_);

            ++first;
        }

        #ifndef DEBUG
            std::thread::id this_id = std::this_thread::get_id();
            std::cout<< "\t [" << this_id << "] end"<<std::endl << std::flush; 
        #endif
    }
};

};
#endif // SCENE_H
