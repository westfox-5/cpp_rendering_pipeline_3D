#pragma once // include only one time at compile-time

namespace rpl {

    template <typename target_t>
    class TargetInterface {
    private:
        int w, h;
    protected:
        target_t** buff;
    public:
        TargetInterface(int width, int height) : w(width), h(height) {}
    public:
        const int ScreenWidth() const { return w;} 
        const int ScreenHeight() const { return h;} 

        virtual void Set(int row, int col, target_t val) const = 0;
        virtual target_t Get(int row, int col) const = 0;

    };
}