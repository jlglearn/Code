#ifndef _BITFIELD_H_
#define _BITFIELD_H_

typedef unsigned int BITWORD;

class BITFIELD
{
    public:
    BITFIELD(void);
    BITFIELD(int nsize);
    ~BITFIELD(void);
    int getBit(int iBit);
    int setBit(int iBit);
    void clearBit(int iBit);
    int count(void);
    void Reset(int newsize);
    
    private:
    BITWORD *pB;
    int size;
    int nset;
    
    void clear(void);
    void resize(int newsize);
};

#endif