#include "circular_buffer.hpp"

void Circular_buffer::put (uint8_t d) {
    buf[wr_idx] = d;

    wr_idx++;
    wr_idx %= SIZE;

    if (wr_idx == rd_idx) full_ = true;
}

uint8_t Circular_buffer::get () {
    uint8_t d = buf[rd_idx];

    rd_idx++;
    rd_idx %= SIZE;
    
    return d;
}

bool Circular_buffer::empty () {return ((wr_idx == rd_idx) and (not full_));}

bool Circular_buffer::full () {return full_;}

Circular_buffer::Circular_buffer (): wr_idx{}, rd_idx{}, full_{false} {}