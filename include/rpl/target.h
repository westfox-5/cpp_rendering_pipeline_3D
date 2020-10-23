#pragma once // include only one time at compile-time

namespace rpl {

    template <typename target_t>
    class TargetInterface {
    public:
        virtual void Set(size_t x, size_t y, target_t t) =0;
        virtual target_t Get(size_t x, size_t y) =0;
    };
}