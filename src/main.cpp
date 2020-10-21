#include <iostream>

#include "rpl/rpl.h"
#include "math.cpp"


void a();

int main(int, char**) {

    a();
}


void a() {
    const rpl::math::vf2d v1 = { 1.0, 2.1 };
    const  rpl::math::vf2d v2 = {1.2f, 2.4f};

    //std::cout << v1 << " + " << v2 << " = " << (v1+v2) << std::endl;
    std::cout << v1 << " + " << v2 << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::math::vf3d v3 = {1.0f, 2.2, 4};
    const rpl::math::vf3d v4 = {1.2f, 2.4f, 12.3};

    //std::cout << v3 << " + " << v4 << " = " << (v3+v4) << std::endl;
        std::cout << v3 << " + " << v4 << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::math::vf4d v5 = {1.0f, 2.0, 4.0, 13};
    const rpl::math::vf4d v6 = {1.2f, 2.4f, 12.3, 0.3};

    // std::cout << v5 << " + " << v6 << " = " << (v5+v6) << std::endl;
    // std::cout << v5 << " / " << v6 << " = " << (v5/v6) << std::endl;
    // std::cout << " mag(v5) " << " = " << mag(v5) << std::endl;
    std::cout << v5 << " + " << v6 << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::math::mi2d mi2d_1 = {1, 2, 3, 4};
    std::cout << mi2d_1 << std::endl;

}