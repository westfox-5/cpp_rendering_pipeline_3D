#pragma once // include only one time at compile-time

namespace rpl {

    template <typename target_t>
    class Target {
    public:
        Target(int32_t width, int32_t height) : w(width), h(height) { }

        ~Target()
        { 
            for(int32_t i=0; i<ScreenHeight(); i++) 
                delete[] buff[i];
            delete buff;
        }

        const int32_t ScreenWidth() { return w;} 
        const int32_t ScreenHeight() { return h;} 


        /* User has to redefine this */
        virtual void InitBuffer() = 0 ;
        virtual void Set(int32_t row, int32_t col, target_t val) = 0;
        virtual target_t Get(int32_t row, int32_t col) = 0;

    protected:
        const bool IsInBound(int32_t row, int32_t col) 
        { return row >= 0 && row < ScreenHeight() && col >= 0 && col < ScreenWidth();} 

    protected:
        target_t** buff;
    private:
        int32_t w, h;
    };
}