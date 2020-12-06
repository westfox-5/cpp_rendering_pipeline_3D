#ifndef MESH_H
#define MESH_H

#include "texture.h"

#include <array>
#include <vector>
#include <algorithm>

namespace rpl {

    struct Vec2d {
    	float x;
        float y;
	};

	struct Vec3d {
    	float x;
    	float y;
    	float z;
	};

    struct Vertex {
        Vec3d p;
        Vec3d np;
        Vec2d t; // texture coord

        Vertex() : p{}, np{}, t{} {}
        Vertex(Vec3d p_, Vec3d np_, Vec2d t_) : p(p_), np(np_), t(t_) {}
    };

    struct Triangle {
        static const size_t MAX_SIZE = 3;

        Vertex points[MAX_SIZE];
    };

    class Mesh {
    public:
        Mesh(std::initializer_list<Triangle> tris) : triangles{tris} {} 
        ~Mesh() { delete texture; }

        void load_texture(const char* filename) {
            texture = new Texture(filename);
        }

        std::vector<Triangle> get_triangles() const {
            return triangles;
        }

        rpl::Pixel sample_pixel(Vertex& v) const {
            return texture->sample_pixel(v.t.x, v.t.y);    
        }
    private:
        std::vector<Triangle> triangles;
        Texture* texture;
    };
}


#endif //MESH_H
