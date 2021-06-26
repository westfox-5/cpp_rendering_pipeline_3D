/**
 * 
 *  [CM0470-2] RENDERING PIPELINE - ASSIGNMENT 3
 *  Davide Volpe - 862989
 * 
 *  Multi Threading @ scene level.
 * 
 *  Compile using the following command in the directory 'assignment_3/scene_level'
 *      g++ main.cpp read-obj.cpp  -o main  -pthread
 *  
 *  Run the program assigning number of objects to render and number of threads to utilize
 *   (e.g.)     ./main 100000 4 
 * 
 * */
 
#include"rasterization.h"
#include"scene.h"
#include"read-obj.h"
#include"../timer.h"
using namespace pipeline3D;

#include<iostream>

using namespace std;



/** ENABLES DEBUGGING LOGGING AND PREVENTS PRINTING THE FINAL SCENE  **/
/** comment out and re-compile the code in order to disable DEBUG    **/
// #define DEBUG






struct my_shader{
        char operator ()(const Vertex &v)  {
        return static_cast<char>((v.z-1)*10.0f+0.5f)%10+'0';
    }
};


int main(int argc, char **argv) {
    const int w=150;
    const int h=50;

    int num_objects = 1;
    int num_worker_threads = 1;
    int num_runs = 1;

    if (argc < 2) {
        std::cout << "Please specify number of objects to render, number of threads and number of runs as command line arguments" << std::endl;
        exit(-1);
    } else if (argc == 2) {
        std::cout << "Executing a single run in single thread mode. Please specify number of threads as command line argument." << std::endl;    
        num_objects = stoi(argv[1]);
    } else if (argc == 3) {
        num_objects = stoi(argv[1]);
        num_worker_threads = stoi(argv[2]);
        std::cout << "Executing a single run in multi thread mode. Number of threads: " << num_worker_threads << std::endl;
    } else if (argc == 4) {
        num_objects = stoi(argv[1]);
        num_worker_threads = stoi(argv[2]);
        num_runs = stoi(argv[3]);
        std::cout << "Running "<< num_runs <<" times in multi thread mode. Number of threads: " << num_worker_threads << std::endl;
    }

    Timer timer;
    std::vector<char> screen(w*h,'.');

    my_shader shader;
    Rasterizer<char> rasterizer;
    rasterizer.set_perspective_projection(-1,1,-1,1,1,2);

    rasterizer.set_target(w,h,&screen[0]);
    
    std::vector<std::vector<std::array<Vertex,3>>> objects;

    Scene<char> scene;
    scene.view_={0.5f,0.0f,0.0f,0.7f,0.0f,0.5f,0.0f,0.7f,0.0f,0.0f,0.5f,0.9f,0.0f,0.0f,0.0f,1.0f};
    scene.n_threads = num_worker_threads;
    
    for (int run = 0; run < num_runs; run++) {
        screen.clear();

        // load the same mesh 'num_objects' times
        std::vector<std::array<Vertex,3>> mesh = read_obj("../cubeMod.obj");

        for (int i=0; i<num_objects; ++i) {
            scene.add_object(Scene<char>::Object(mesh, shader));
        }
        
        std::cout << "START rendering with "<< num_worker_threads <<" threads"<< std::endl;
        timer.reset();

        scene.render(rasterizer);

        std::cout << "END rendering "<< num_objects <<" with "<< num_worker_threads << " threads ["<< timer.elapsed() <<" sec]"<< std::endl;
    }
    
    #ifndef DEBUG
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
    #endif
    return 0;
}