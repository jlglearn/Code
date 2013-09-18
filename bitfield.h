#ifndef _BITFIELD_H_
#define _BITFIELD_H_

class BITFIELD
{
    public:
    BITFIELD(int nsize);
    ~BITFIELD(void);
    int getBit(int iBit);
    void setBit(int iBit);
    
    private:
    int *pB;
    int size;
    
    void resize(int newsize);
};

#endif