#include <string>
#include <cstdlib>
#include <cstring>
#include "debug.h"
#include "bitfield.h"

#define INTBITS (8 * sizeof(int))

BITFIELD::BITFIELD(int nsize)
{
    size = 0;
    pB = (int *)0;
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

void BITFIELD::setBit(int iBit)
{
    int i = iBit / INTBITS;
    int j = iBit % INTBITS;
    int k = 1 << j;
    
    if ( i >= size )
    {
        resize(size * 2);
    }
    
    pB[i] |= k;
}


// newsize is given in number of bits desired in bitfield
void BITFIELD::resize(int newsize)
{
    int k;
    
    // round up to an even power of 2, with a minimum of 512
    for (k = 512; k < newsize; k *= 2) ;
    
    int byteSize = k / 8;
    int *pNew = (int *) malloc(byteSize);    
    memset((void *) pNew, 0, byteSize);
    
    if ( pB )
    {
        memcpy((void *) pNew, (void *) pB, size/8 );
        free(pB);
    }
    
    pB = pNew;
    size = k;
}