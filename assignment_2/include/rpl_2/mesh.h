#pragma once

#include <vector>
#include <algorithm>

#include "texture.h"
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

        Vertex(Vec3d p, Vec3d np, Vec2d t);
    };

    struct Triangle {
        Vertex points[3];
    };

    class Mesh {
    public:
        Mesh();
        ~Mesh() { delete texture; }

        void load_texture(const char* filename) {
            texture = new Texture(filename);
        }

        void add_triangle(Triangle t) {
            triangles.push_back(t);
        }

        std::vector<Triangle> get_sorted_triangles() const {
            std::vector<Triangle> sorted;
            std::copy(triangles.begin(),triangles.end(), sorted);
            std::sort(sorted.begin(), sorted.end(), sortFn);
            return sorted;
        }

        rpl::Pixel sample_pixel(Vertex& v) const {
            return texture->sample_pixel(v.t.x, v.t.y);    
        } 
    private:
        std::vector<Triangle> triangles;
        Texture* texture;

        static bool sortFn( Vertex i, Vertex j) {
            return (i.p.y < j.p.y); // y-ascending order
        }
    };
}