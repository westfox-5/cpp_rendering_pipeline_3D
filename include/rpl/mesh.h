#pragma once // include only one time at compile-time

#include <vector>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "math.h"

namespace rpl::Mesh {
    struct Triangle {
        std::array<rpl::Math::Vector3D, 3> points;

        Triangle(std::array<rpl::Math::Vector3D, 3> p): points(p) {}
    };

    struct Mesh {
        std::vector<rpl::Mesh::Triangle> triangles;

        ~Mesh() {}
    };
}

