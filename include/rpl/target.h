#pragma once

#include <vector>

namespace rpl {

    template <typename target_t>
    class Target {
    public:
        Target(size_t width, size_t height) :
            worldW(width), 
            worldH(height),
            drawTarget{0,0},          // starts at 0,0
            drawTargetW(20),        // arbitrary
            drawTargetH(20)        // arbitrary
        {

            // init buffers
            this->buff = new target_t[ width*height ];
            this->z_buff = new size_t[ width*height ];

        }
        ~Target() 
        { 
            delete[] this->buff;
            delete[] this->z_buff;
        }

        size_t ScreenWidth() { return worldW; }
        size_t ScreenHeight() { return worldH; }

        void Render() 
        {

            for(size_t x = 0; x < worldW; ++x) {
                
                // in-drawTarget check
                if (  x >= this->drawTarget.X() && x <= (this->drawTarget.X()+this->drawTargetH) ) {

                    for(size_t y = 0; y < worldH; ++y) {

                        if ( y >= this->drawTarget.Y() && y <= (this->drawTarget.Y()+this->drawTargetW) ) {

                            // TODO
                        }
                    }

                }
            }

        }

    public:
        virtual void set(size_t x, size_t y, target_t p) { 
            if (x >= 0 && x < worldW && y >= 0 && y < worldH) {
                buff[y * worldW + x] = p;
                // TODO z_buff
            }
        } 
    private:
        size_t worldW, worldH;
        rpl::Math::Vector<size_t, 2> drawTarget;
        size_t drawTargetW, drawTargetH;

        float aspectRatio;

        target_t* buff;
        size_t* z_buff;
        

    };
}