#include <string>
#include <cstdlib>
#include <cstring>
#include "debug.h"
#include "bitfield.h"

static const int INTBITS = 8 * sizeof(int);
static const int MINBITFIELDSIZE = INTBITS; // minimum size of allocated bitfield (in bits)

void BITFIELD::clear(void)
{
    size = 0;
    nset = 0;
    pB = (BITWORD *)0;
}

BITFIELD::BITFIELD(void)
{
    clear();
    resize(0);
}

BITFIELD::BITFIELD(int nsize)
{
    clear();
    resize(nsize);
}

BITFIELD::~BITFIELD(void)
{
    if (pB) free(pB);
}

int BITFIELD::getBit(int iBit)
{
    if (iBit >= (size * INTBITS))
        return 0;
    
    int i = iBit / INTBITS;
    int j = iBit % INTBITS;
    int k = 1 << j;
    
    return ((pB[i] & k) != 0);
}

void BITFIELD::clearBit(int iBit)
{
    if (iBit < (size * INTBITS))
    {
        int i = iBit / INTBITS;
        int j = iBit % INTBITS;
        int k = ~(1 << j);
        pB[i] &= k;
    }
}

int BITFIELD::count(void)
{
    return nset;
}

int BITFIELD::setBit(int iBit)
{
    int i = iBit / INTBITS;
    int j = iBit % INTBITS;
    int k = 1 << j;
    
    if ( i >= size )
    {
        resize(size * 2);
    }
    
    if (!(pB[i] & k))
    {
        pB[i] |= k;
        nset++;
    }
    
    return nset;
}

void BITFIELD::Reset(int newsize)
{
    if (nset > 0)
    {
        delete pB;
        clear();
    }
    resize(newsize);
}

// newsize is given in number of bits desired in bitfield
void BITFIELD::resize(int newsize)
{
    int k;
    
    // round up to an even power of 2, with a minimum of MINBITFIELDSIZE bits
    for (k = MINBITFIELDSIZE; k < newsize; k *= 2) ;
    
    int byteSize = k / 8;
    BITWORD *pNew = (BITWORD *) malloc(byteSize);    
    memset((void *) pNew, 0, byteSize);
    
    if ( pB )
    {
        memcpy((void *) pNew, (void *) pB, size/8 );
        free(pB);
    }
    
    pB = pNew;
    size = k;
}