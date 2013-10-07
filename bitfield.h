#ifndef _BITFIELD_H_
#define _BITFIELD_H_

class BITFIELD
{
    public:
    BITFIELD(int nsize);
    ~BITFIELD(void);
    int getBit(int iBit);
    int setBit(int iBit);
    int count(void);
    
    private:
    int *pB;
    int size;
    int nset;
    
    void resize(int newsize);
};

#endif