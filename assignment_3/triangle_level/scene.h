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

          void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view, int n_threads) { pimpl->render(rasterizer,view,world_, n_threads); }

          std::array<float,16> world_;

    private:

        struct Object_impl {
          virtual ~Object_impl() {}
          virtual void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view, const std::array<float,16>& world, int n_threads)=0;
        };

        template<class Mesh, class Shader, class... Textures>
        class concrete_Object_impl : public Object_impl {
        public:
            concrete_Object_impl(Mesh &&mesh, Shader &&shader, Textures&&... textures ) :
                mesh_(std::forward<Mesh>(mesh)), shader_(std::forward<Shader>(shader)), textures_(std::forward<Textures>(textures)...) {}

            void render(Rasterizer<target_t>& rasterizer, const std::array<float,16>& view, const std::array<float,16>& world, int n_threads) override {
                int num_faces = mesh_.size();

                // LOAD BALANCING  PER THREAD
                
                const int load_per_thread = (int)(std::floor(num_faces / n_threads));
                int num_remaining_tris = num_faces % n_threads;
                int num_faces_assigned_so_far = 0;

                // WORKER CREATION

                std::vector<std::thread> workers(n_threads);

                for (int thread_count = 0; thread_count < n_threads; ++thread_count) {
                    
                    // if the modulus is > 0, num of triangles is not divisible by n_threads
                    // so we need to assign more triangles to each thread.
                    //
                    // by assigning one more to each, we are sure to correctly balance
                    // all worker's load
                    int num_faces_to_assign = load_per_thread;
                    if (num_remaining_tris > 0) {
                        ++num_faces_to_assign;
                        --num_remaining_tris;
                    }
                    
                    auto first = mesh_.begin() + num_faces_assigned_so_far;
                    auto last = first + num_faces_to_assign; // size from first to last is num_objects_to_assign

                    // if calculus overflows triangles, stop at last triangle
                    if ( num_faces_assigned_so_far + num_faces_to_assign > num_faces) {
                        last = mesh_.end();
                    }
                    
                    // increment the number of triangles assigned so far
                    num_faces_assigned_so_far += num_faces_to_assign;

                    // create thread with the worker function
                    std::thread thread([&]{  
                        std::unique_lock<std::mutex> lck(mtx);
                        while (first != last) {

                            auto &t = *first;
                            auto v1=t[0];
                            auto v2=t[1];
                            auto v3=t[2];
                            transform(world,v1);
                            transform(view,v1);
                            transform(world,v2);
                            transform(view,v2);
                            transform(world,v3);
                            transform(view,v3);
                            rasterizer.render_vertices(v1,v2,v3,shader_);


                            ++first;
                        }
                        
                        #ifdef DEBUG
                            std::thread::id this_id = std::this_thread::get_id();
                            std::cout<< "\t thread [" << this_id << "] end"<<std::endl << std::flush; 
                        #endif
                        lck.unlock();
                    });

                    workers.push_back( std::move(thread) );

                    #ifdef DEBUG
                        std::cout << "\t load assigned: "<<num_faces_to_assign<< " - total so far: " << num_faces_assigned_so_far << std::endl << std::flush;
                    #endif
                }

                for (std::thread &t: workers) { 
                    if (t.joinable()) {
                        t.join();
                    }
                }
            }


        private:	
            std::mutex mtx;

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
        for (auto& o : objects) {
            std::unique_lock<std::mutex> lck1(mtx2);
            o.render(rasterizer,view_, n_threads);
            lck1.unlock();
        }
    }

private:

    std::mutex mtx2;
    std::vector<Object> objects;
};

};
#endif // SCENE_H
