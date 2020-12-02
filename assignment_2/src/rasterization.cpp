#include"rasterization.h"
using namespace pipeline3D;

#include<iostream>
#include<chrono>


int main() {
    const int w=1420;
    const int h=1420;

    Rasterizer<Pixel> rasterizer;
    rasterizer.set_perspective_projection(-1,1,-1,1,1,2);

    Texture texture("../textures/texture_3.jpg");
    rasterizer.set_texture(&texture);

    std::vector<Pixel> target(w*h, {0,0,0}); // all black
    rasterizer.set_target(w, h, target);

    constexpr float slope=-0.2f;
    Vertex 
        v1={1,-1,1.5f+slope*(1-1)  ,0,0,0, 1,0},
        v2={1,1,1.5f+slope*(1+1)   ,0,0,0, 1,1},
        v3={-1,1,1.5f+slope*(-1+1),0,0,0, 0,1},
        v4={-1,-1,1.5f+slope*(-1-1),0,0,0, 0,0};

    rasterizer.render_triangle(v1,v2,v3);
    rasterizer.render_triangle(v4,v1,v3);

    rasterizer.save_to_file("../out/result.bmp");

    return 0;
}