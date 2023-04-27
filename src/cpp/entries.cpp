#include <entries.h>

void writeEntry(uint32_t p_time, uint32_t p_user, uint8_t p_r, uint8_t p_g, uint8_t p_b, uint16_t p_x, uint16_t p_y, PackedEntry *entry)
{
    entry->time = p_time;
    entry->user = p_user;
    entry->r = p_r;
    entry->g = p_g;
    entry->b = p_b;

    packCords(p_x, p_y, &(entry->cordUpper), &(entry->cordLower));
}

void packCords(uint16_t x, uint16_t y, uint8_t *cordUpper, uint16_t *cordLower)
{
    uint32_t packed = 0;
    uint16_t maskXY = 0b0000011111111111;
    uint32_t maskLower = 0b1111111111111111;
    uint32_t maskUpper = 0b111111110000000000000000;
#ifdef DEBUG_CORD_PACKING
    if ( (x & maskXY) != x )
    {
        printf("X is greater than 2048");
    }
    if ( (y & maskXY) != y )
    {
        printf("Y is greater than 2048");
    }
#endif
    x = x & maskXY;
    y = y & maskXY;
    packed = ((uint32_t)y) | (((uint32_t)x) << 11);
#ifdef DEBUG_CORD_PACKING
    printf("Packed: %x\n", packed);
#endif
    (*cordLower) = packed & maskLower;
    (*cordUpper) = (packed & maskUpper) >> 16;
}

void unpackCords(uint8_t cordUpper, uint16_t cordLower, uint16_t *x, uint16_t *y)
{
    uint32_t packed;
    uint16_t maskXY = 0b0000011111111111;
    packed = (((uint32_t)cordUpper) << 16) | ((uint32_t)cordLower);
#ifdef DEBUG_CORD_PACKING
    printf("Packed: %x\n", packed);
#endif
    (*x) = packed >> 11;
    (*y) = packed & maskXY;
}

void printPackedEntry(PackedEntry *entry)
{
    uint16_t x, y;
    unpackCords(entry->cordUpper, entry->cordLower, &x, &y);
    printf("Time: %u\nUser: %u\nColor: #%x%x%x\n(%d, %d)\n", entry->time, entry->user, entry->r, entry->g, entry->b, x, y);
}
/* Code used to test packing and unpacking
{
    Entry *e = (Entry *)malloc(sizeof(Entry));
    std::printf("Entry Size: %d\n", (int)sizeof(Entry));


    uint16_t testX;
    uint16_t testY;
    uint8_t upper;
    uint16_t lower;
    packCords(0b11111011111, 0b10001010001, &upper, &lower);
    printf("0x%x, 0x%x", upper, lower);
    for ( uint16_t x = 0; x < 2000; x++ )
    {
        for ( uint16_t y = 0; y < 2000; y++ )
        {
            packCords(x, y, &upper, &lower);
            unpackCords(upper, lower, &testX, &testY);
            if ( !((x == testX) && (y == testY)) )
            {
                printf("(%d, %d)->(%d, %d)\n", x, y, testX, testY);
            }
        }
    }
}
*/

bool Entry::operator==(const Entry &other) const
{
    if ( this->time != other.time )
    {
        return false;
    }
    if ( this->user != other.user )
    {
        return false;
    }
    if ( this->r != other.r )
    {
        return false;
    }
    if ( this->g != other.g )
    {
        return false;
    }
    if ( this->b != other.b )
    {
        return false;
    }
    if ( this->x != other.x )
    {
        return false;
    }
    if ( this->y != other.y )
    {
        return false;
    }
    return true;
}