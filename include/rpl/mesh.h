#pragma once // include only one time at compile-time

#include <vector>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "math.h"

namespace rpl {

    namespace Mesh {

        struct Triangle {
            rpl::Math::Vector3D points[3];
        };

        struct Mesh {
            std::vector<rpl::Mesh::Triangle> triangles; // indexed by faces[i]

            ~Mesh() {}
        };
    }
}

