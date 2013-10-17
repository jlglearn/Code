#ifndef _BITFIELD_H_
#define _BITFIELD_H_

typedef unsigned int BITWORD;

class BITFIELD
{
    public:
    BITFIELD(int nsize);
    ~BITFIELD(void);
    int getBit(int iBit);
    int setBit(int iBit);
    int count(void);
    
    private:
    BITWORD *pB;
    int size;
    int nset;
    
    void resize(int newsize);
};

#endif