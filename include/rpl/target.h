#pragma once // include only one time at compile-time

namespace rpl {

    template <typename target_t>
    class TargetInterface {
    public:
        virtual void Set(int row, int col, target_t val) = 0;
        virtual target_t Get(int row, int col) = 0;

    };
}