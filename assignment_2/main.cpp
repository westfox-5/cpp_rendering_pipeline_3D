#include "include/rpl_2/engine.h"

using namespace rpl;


struct MyVertex : public Vertex {

    // other information

};



int main() {
    const int w=1420;
    const int h=1420;

    const char* texture_filename = "textures/texture_2.bmp";

    Engine<Pixel> engine;
    engine.set_perspective_projection(-1,1,-1,1,1,2);

    std::vector<Pixel> target(w*h, {0,0,0}); // all black
    engine.set_target(w, h, target);

    Mesh m;

    m.load_texture(texture_filename);

    Triangle t1 = {{
        Vertex({1,-1, 2.f }, {0,0,0}, {1,0}),
        Vertex({1,-1, 2.f }, {0,0,0}, {1,0}),
        Vertex({1,-1, 2.f }, {0,0,0}, {1,0}),
    }};

    m.add_triangle( t1 );

    // constexpr float slope=-0.2f;
    // Vertex 
    //     v1={1,-1, 2.f  ,0,0,0, 1,0},
    //     v2={1,1,  2.f   ,0,0,0, 1,1},
    //     v3={-1,1, 2.f  ,0,0,0, 0,1},
    //     v4={-1,-1,2.f ,0,0,0, 0,0};

    // engine.render_triangle(v1,v2,v3);
    // engine.render_triangle(v4,v1,v3);

    engine.save_to_file("out/result.jpg");

    return 0;
}