#pragma once // include only one time at compile-time

namespace rpl {

    template <typename target_t>
    class Target {
    public:
        Target(int width, int height) : w(width), h(height) {
            buff = new target_t *[h];
            for (int r = 0; r < h; r++)
            {
                target_t *subVector = new target_t[w];
                for (int c = 0; c < w; c++)
                    subVector[c] = '.';
                buff[r] = subVector;
            }
        }
        ~Target()
        { 
            for(int i=0; i<ScreenHeight(); i++) 
                delete[] buff[i];
            delete buff;
        }

        const int ScreenWidth() const { return w;} 
        const int ScreenHeight() const { return h;} 

        virtual void Set(int row, int col, target_t val) const = 0;
        virtual target_t Get(int row, int col) const = 0;
    protected:
        target_t** buff;
    private:
        int w, h;
    };
}