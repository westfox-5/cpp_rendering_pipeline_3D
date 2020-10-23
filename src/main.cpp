#include <iostream>

#include "rpl/engine.h"

class CharBuffer : public rpl::TargetInterface<char> {
public:
    CharBuffer() {};
    CharBuffer(int w, int h): width(w), height(h), buff(w*h) {}

public:

    void Set(size_t x, size_t y, char t) {
        buff[x * width + y] = t;
    }

    char Get(size_t x, size_t y) {
        return buff[x * width + y];
    }


private:
    int width, height;
    std::vector<char> buff;

};



class MyEngine : public rpl::Engine<char> {
    private:
        int i = 0;

    public:
    bool OnCreate() final {

        // crea triangoli

        return true;
    }

    // bool OnUpdate() final {
    //     return true;
    // }

    void OnDestroy() final {

    }
};



int main(int, char**) {
    CharBuffer myBuff(150, 50);

    MyEngine myEngine;
    if (myEngine.Build(150, 50, &myBuff)) {
        //myEngine.Print();
    }


    // stampa
    // salva su file
    // ...


    return 0;
}






// void a() {
//     const rpl::Math::vf2d v1 = { 1.0, 2.1 };
//     const  rpl::Math::vf2d v2 = {1.2f, 2.4f};

//     //std::cout << v1 << " + " << v2 << " = " << (v1+v2) << std::endl;
//     std::cout << v1 << " + " << v2 << std::endl;
//     std::cout << std::endl;
//     std::cout << std::endl;

//     const rpl::Math::vf3d v3 = {1.0f, 2.2, 4};
//     const rpl::Math::vf3d v4 = {1.2f, 2.4f, 12.3};

//     //std::cout << v3 << " + " << v4 << " = " << (v3+v4) << std::endl;
//         std::cout << v3 << " + " << v4 << std::endl;

//     std::cout << std::endl;
//     std::cout << std::endl;

//     const rpl::Math::vf4d v5 = {1.0f, 2.0, 4.0, 13};
//     const rpl::Math::vf4d v6 = {1.2f, 2.4f, 12.3, 0.3};

//     // std::cout << v5 << " + " << v6 << " = " << (v5+v6) << std::endl;
//     // std::cout << v5 << " / " << v6 << " = " << (v5/v6) << std::endl;
//     // std::cout << " mag(v5) " << " = " << mag(v5) << std::endl;
//     std::cout << v5 << " + " << v6 << std::endl;
//     std::cout << std::endl;
//     std::cout << std::endl;

//     const rpl::Math::Matrix<float,2> mf2d_1 = {
//         1, 2, 
//         3, 4
//     };
//     const rpl::Math::Matrix<float,2> mf2d_2 = {
//         1, 2,
//         3, 4};
//     std::cout << (mf2d_1*mf2d_2) << std::endl;

// }