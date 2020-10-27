#pragma once // include only one time at compile-time

#include "math.h"

namespace rpl {

    template<typename target_t>
    class FragmentShader {
    public:
        virtual bool apply(target_t &target, rpl::Math::Vector3D light, rpl::Math::Vector3D normal)  = 0;
    };


}