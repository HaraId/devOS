#ifndef _DHARMA_MOUSE_H
#define _DHARMA_MOUSE_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"

#include "drivers/driver.h"

#include "tx/stdio.h"

class PS2MouseDriver : public InterruptHandler, public Driver
{
public:
    PS2MouseDriver();
    ~PS2MouseDriver();

    
// InterruptHandler interface:
    uint32_t handle(uint32_t esp) override;
    

// Driver interface:
    virtual void Activate() override;
    virtual int Reset() override;
    virtual void Deactivate() override;

    
private:
    Port8Bit m_dataPort;
    Port8Bit m_commandPort;
    
    uint8_t m_buff[3];
    uint8_t m_off;
    uint8_t m_buttonsFlag;
    
};

#endif
