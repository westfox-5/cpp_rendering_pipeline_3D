#pragma once // include only one time at compile-time


namespace rpl {

    template<typename target_t>
    class FragmentShader {
    public:
        FragmentShader();
        ~FragmentShader();
    public:
        virtual target_t apply(target_t p)  = 0;
    };

}