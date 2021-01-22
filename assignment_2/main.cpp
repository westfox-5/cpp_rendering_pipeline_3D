#include "include/rpl_2/engine.h"

#include  <iostream>

using namespace rpl;


struct MyVertex : public rpl::Vertex {
    // other information
    MyVertex() : Vertex() {}
    MyVertex(Vec3d p_, Vec3d np_, Vec2d t_) : Vertex(p_, np_, t_) {}
};



int main() {
    const int w=1024;
    const int h=1024;

    Engine<Pixel> engine;
    engine.set_perspective_projection(-1,1,-1,1,1,2);

    std::vector<Pixel> target(w*h, {0,0,0}); // all black
    engine.set_target(w, h, target);

    /*
        FIRST MESH 
        Position at the RIGHT side of the screen
    */
    MyVertex 
    v1({2,-1, 2.f }, {0,0,0}, {1,0}),
    v2({2, 1, 2.f }, {0,0,0}, {1,1}),
    v3({0, 1, 2.f }, {0,0,0}, {0,1}),
    v4({0,-1, 2.f }, {0,0,0}, {0,0});
    rpl::Triangle 
    t1 = {{ v1, v2, v3 }},
    t2 = {{ v4, v1, v3 }};

    // Create Mesh object
    rpl::Mesh mesh({t1, t2});
    mesh.load_texture("textures/texture_1.bmp");
    engine.render_mesh(mesh);

    /*
        SECOND MESH 
        Position at the LEFT side of the screen
    */
   
    rpl::Vertex 
    u1({ 0,-1, 2.f }, {0,0,0}, {1,0}),
    u2({ 0, 1, 2.f }, {0,0,0}, {1,1}),
    u3({-2, 1, 2.f }, {0,0,0}, {0,1}),
    u4({-2,-1, 2.f }, {0,0,0}, {0,0});

    rpl::Triangle 
    r1 = {{ u1, u2, u3 }},
    r2 = {{ u4, u1, u3 }};

    // Create Mesh object
    rpl::Mesh mesh2({r1, r2});
    mesh2.load_texture("textures/texture_3.bmp");    
    engine.render_mesh(mesh2);

    engine.save_to_file("./out/result.bmp");

    return 0;
}