#include <iostream>

#include "rpl/engine.h"
#include "math.cpp"


void a();

class MyEngine : public rpl::Engine<char> {
    private:
        int i = 0;
    public:
    bool OnCreate() final {

        return true;
    }

    bool OnUpdate() final {

        Draw(1,1, 'A');

        //getch();

        //if (i == 100) return false;
        return true;
    }

    void OnDestroy() final {

    }
};


int main(int, char**) {
    //a();


    MyEngine e;
    if (e.Build(20, 20)) {
        e.Run();
    }

    return 0;
}






void a() {
    const rpl::Math::vf2d v1 = { 1.0, 2.1 };
    const  rpl::Math::vf2d v2 = {1.2f, 2.4f};

    //std::cout << v1 << " + " << v2 << " = " << (v1+v2) << std::endl;
    std::cout << v1 << " + " << v2 << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::Math::vf3d v3 = {1.0f, 2.2, 4};
    const rpl::Math::vf3d v4 = {1.2f, 2.4f, 12.3};

    //std::cout << v3 << " + " << v4 << " = " << (v3+v4) << std::endl;
        std::cout << v3 << " + " << v4 << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::Math::vf4d v5 = {1.0f, 2.0, 4.0, 13};
    const rpl::Math::vf4d v6 = {1.2f, 2.4f, 12.3, 0.3};

    // std::cout << v5 << " + " << v6 << " = " << (v5+v6) << std::endl;
    // std::cout << v5 << " / " << v6 << " = " << (v5/v6) << std::endl;
    // std::cout << " mag(v5) " << " = " << mag(v5) << std::endl;
    std::cout << v5 << " + " << v6 << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    const rpl::Math::Matrix<float,2> mf2d_1 = {
        1, 2, 
        3, 4
    };
    const rpl::Math::Matrix<float,2> mf2d_2 = {
        1, 2,
        3, 4};
    std::cout << (mf2d_1*mf2d_2) << std::endl;

}