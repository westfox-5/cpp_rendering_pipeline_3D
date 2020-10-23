#pragma once // include only one time at compile-time

#include "pipe.h"
#include "math.h"
#include "target.h"
#include "shader.h"

namespace rpl {
    
    template <typename target_t>
    class Engine {
    public:
        bool Build(int32_t width, int32_t height, TargetInterface<target_t>* target) {
            can_run = true;

            can_run |= pipe->Build(width, height, target);


            return can_run;
        }


    public:
        ~Engine() {
            delete this->pipe;
        }
    public:
        virtual bool OnCreate()  /*user defined*/   = 0;
        // virtual bool OnUpdate()  /*user defined*/   = 0;
        virtual void OnDestroy() /*user defined*/   = 0;
    public:

        void Run() {

            // initialize user content
            if (!OnCreate())  {
                can_run = false;
            }

            // while (can_run) {
            //     if (!OnUpdate()) {
            //         can_run = false;
            //     }

            //     // display!
            //     pipe.ClearBuffer();
            //     pipe.Render();
            // }


            // apply transformation matrixxxx

            if (can_run) {
                pipe.Render();
            }

            // end of the games
            OnDestroy();
        }

    protected:
        rpl::PipeLine<target_t>* pipe;
    
    private:
        bool can_run;
    };



}