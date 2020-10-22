#pragma once // include only one time at compile-time

#include "math.h"
#include "target.h"

namespace rpl {
    
    template<typename target_t>
    class Engine {
    public:
        bool Build(size_t width, size_t height) {
            can_run = true;

            // initialize target            
            
            this->target =  new rpl::Target<target_t>(width, height);

            return true;
        }


    public:
        ~Engine() {
            delete this->target;
        }
    public:
        virtual bool OnCreate()  /*user defined*/   = 0;
        virtual bool OnUpdate()  /*user defined*/   = 0;
        virtual void OnDestroy() /*user defined*/   = 0;
    public:
        void Draw(size_t x, size_t y, target_t p);


        void Run() {

            std::cout<< "here" << std::endl;
            // first of all, initialize user content
            if (!OnCreate())  {
                can_run = false;
            }

            while (can_run) {

                // display text!
                this->target->Render();

                if (!OnUpdate()) {
                    can_run = false;
                }
            }

            // end of the games
            OnDestroy();
        }

    private:
        rpl::Target<target_t>* target;
        bool can_run;
    };


    template <typename target_t>
    void rpl::Engine<target_t>::Draw(size_t x, size_t y, target_t p) {
        if (!p) return;

        target->set(x, y, p);
    }


}