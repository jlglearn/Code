#include <string>
#include <cstdlib>
#include <cstring>
#include "debug.h"
#include "bitfield.h"

#define INTBITS (8 * sizeof(int))

BITFIELD::BITFIELD(int nsize)
{
    int k;
    for (k = 2; k < nsize; k *= 2) ;
    size = 0;
    pB = (int *)0;
    resize(k);
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


void BITFIELD::resize(int newsize)
{
    int aSize = newsize / INTBITS;
    int *pNew = (int *) malloc(aSize);
    memset((void *) pNew, 0, aSize);
    
    if ( pB )
    {
        memcpy((void *) pNew, (void *) pB, size / INTBITS);
        free(pB);
    }
    
    pB = pNew;
    size = newsize;
}