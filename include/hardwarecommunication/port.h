#ifndef __DHARMA_PORT_H
#define __DHARMA_PORT_H

#include <common/types.h>

/*
class Port
{
protected:
    Port(uint16_t portnum);
    ~Port();
    
    uint16_t portnumber;
    
};
*/

class Port8Bit
{
public:
    Port8Bit(uint16_t portnum);
    ~Port8Bit();
    
    void Write(uint8_t data);
    uint8_t Read();
    
private:
    uint16_t portnumber;
};

class Port8BitSlow
{
public:
    Port8BitSlow(uint16_t portnum);
    ~Port8BitSlow();
    
    void Write(uint8_t data);
    
private:
    uint16_t portnumber;
    
};






class Port16Bit
{
public:
    Port16Bit(uint16_t portnum);
    ~Port16Bit();

    void Write(uint16_t data);
    uint16_t Read();
    
private:
    uint16_t portnumber;
};

class Port32Bit
{
public:
    Port32Bit(uint16_t portnum);
    ~Port32Bit();
    
    void Write(uint32_t data);
    uint32_t Read();
    
private:
    uint16_t portnumber;
};


#endif
