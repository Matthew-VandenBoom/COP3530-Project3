#pragma once

//#define DEBUG_CORD_PACKING

#include <cstdint>
#include <iostream>

#ifdef DEBUG_CORD_PACKING
#include <iostream>
#endif

struct PackedEntry
{
    uint32_t time;
    uint32_t user;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // x and y cordinates are each 0-1999, which can be represented in 11 bits each
    // if they were each given 16 bits, 10 bits per entry would be wasted
    // there are close to 150 million records, so this waste will add up
    // we will pack them to save space

    // the bottom 11 bits of cordLower will directly correspond to y
    // the bottom 6 bits of cordUpper concatenated with the upper 5 bits
    // of cordLower will give x
    uint8_t cordUpper;
    uint16_t cordLower;
    // we will be ignoring the x2, y2 columns of the original data because they are rare and will waste a lot of space
} __attribute__((packed));

void writeEntry(uint32_t time, uint32_t user, uint8_t r, uint8_t g, uint8_t b, uint16_t x, uint16_t y, PackedEntry *entry);

void packCords(uint16_t x, uint16_t y, uint8_t *cordUpper, uint16_t *cordLower);

void unpackCords(uint8_t cordUpper, uint16_t cordLower, uint16_t *x, uint16_t *y);

void printPackedEntry(PackedEntry *entry);

struct Entry
{
    uint32_t time, user;
    uint8_t r, g, b;
    uint16_t x, y;

    Entry(PackedEntry *e)
    {
        this->time = e->time;
        this->user = e->user;
        this->r = e->r;
        this->g = e->g;
        this->b = e->b;
        unpackCords(e->cordUpper, e->cordLower, &(this->x), &(this->y));
    }
    Entry(uint32_t time, uint32_t user, uint8_t r, uint8_t g, uint8_t b, uint16_t x, uint16_t y) : time(time), user(user), r(r), g(g), b(b), x(x), y(y) {}
    Entry() : Entry(0, 0, 0, 0, 0, 0, 0) {}
    void Print() { printf("Time: %u\nUser: %u\nColor: #%x%x%x\n(%d, %d)\n", time, user, r, g, b, x, y); }
};
