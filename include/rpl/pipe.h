#pragma once

#include <vector>

#include "target.h"
#include "shader.h"

namespace rpl {
    template <typename target_t>
    class PipeLine {
    public:
        PipeLine() :
            width(0),
            height(0),
            
            target(nullptr),
            shader(nullptr),
            
            buff(nullptr) {}
        ~PipeLine() {}


        bool Build(int32_t w, int32_t h, TargetInterface<target_t>* t) {
            width = w; height = h;

            target = t;

            buff = new target_t[width*height];
            return true;
        }

        void Render() 
        {

            // write to target
            for(size_t x = 0; x < width; ++x) {
                for(size_t y = 0; y < height; ++y) {

                    target_t p = buff[x * width + y];
                    p = shader->apply(p);
                    target->Set(x,y, p);
                }
            }
        }
    private:
        int32_t width, height;
        target_t* buff;
    private:
        TargetInterface<target_t>* target;
        FragmentShader<target_t>* shader;
        // rpl::Target<target_t> drawTarget = nullptr;

    public:
        virtual void Draw(size_t x, size_t y, target_t p) { 
            if (x >= 0 && x < width && y >= 0 && y < height) {
                // drawTarget.Set(x,y,p);
                buff[x*width + y] = p;
             }
        }

    };
  

}