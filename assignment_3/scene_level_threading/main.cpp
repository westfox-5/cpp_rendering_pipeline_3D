#include"rasterization.h"
#include"scene.h"
#include"read-obj.h"
#include"../timer.h"
using namespace pipeline3D;

#include<iostream>

using namespace std;


    // g++ main.cpp read-obj.cpp -Wall  -o main  -pthread

    struct my_shader{
         char operator ()(const Vertex &v)  {
            return static_cast<char>((v.z-1)*10.0f+0.5f)%10+'0';
        }
    };


    int main() {
        const int w=150;
        const int h=50;

        const int num_objects = 654321;

        const int num_worker_threads = 1;



        Timer timer;

        my_shader shader;
        Rasterizer<char> rasterizer;
        rasterizer.set_perspective_projection(-1,1,-1,1,1,2);

        std::vector<char> screen(w*h,'.');
        rasterizer.set_target(w,h,&screen[0]);

        std::vector<std::vector<std::array<Vertex,3>>> objects;
        
        

        Scene<char> scene;
        scene.view_={0.5f,0.0f,0.0f,0.7f,0.0f,0.5f,0.0f,0.7f,0.0f,0.0f,0.5f,0.9f,0.0f,0.0f,0.0f,1.0f};
        scene.num_threads_ = num_worker_threads;
        
        // load one mesh
        std::vector<std::array<Vertex,3>> mesh = read_obj("../cubeMod.obj");

        std::cout << "START adding "<< num_objects <<" objects"<< std::endl;
        timer.reset();
        
        for (int i=0; i<num_objects; ++i) {
            scene.add_object(Scene<char>::Object(mesh, shader));
        }
        

        
        std::cout << "END adding "<< num_objects <<" objects ["<< timer.elapsed() <<" sec]"<<std::endl;

        std::cout << "START rendering with "<< scene.num_threads_ <<" threads"<< std::endl;
        timer.reset();

        scene.render(rasterizer);


        std::cout << "END rendering with "<< scene.num_threads_ << " threads ["<< timer.elapsed() <<" sec]"<< std::endl;

/*

        // print out the screen with a frame around it
        std::cout << '+';
        for (int j=0; j!=w; ++j) std::cout << '-';
        std::cout << "+\n";

        for (int i=0;i!=h;++i) {
            std::cout << '|';
            for (int j=0; j!=w; ++j) std::cout << screen[i*w+j];
            std::cout << "|\n";
        }

        std::cout << '+';
        for (int j=0; j!=w; ++j) std::cout << '-';
        std::cout << "+\n";

*/
        return 0;
    }

