#pragma once //единичное выполнение
#include <cstdint> // чтобы включить uint8_t сюда

constexpr uint8_t SIZE{32};

class Circular_buffer {
    public:
        void put(uint8_t);
        uint8_t get();
        bool empty();
        bool full();

        Circular_buffer();
    private:
        uint8_t buf[SIZE];
        uint8_t wr_idx;
        uint8_t rd_idx;
        bool full_;
};