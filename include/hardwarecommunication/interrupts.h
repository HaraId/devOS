#ifndef __DHARMA_INTERRUPTS_H_
#define __DHARMA_INTERRUPTS_H_

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/gdt.h"

//extern "C" void __cxa_pure_virtual() { while(1);}


typedef struct _RIDT_STRUCT
{
    uint16_t  tableLimit;
    uint32_t  tableAddress;
    
} __attribute__((packed)) RIDT_STRUCT, *PRIDT_STRUCT;

class InterruptManager;

class InterruptHandler
{
    friend class InterruptManager;
public:
    uint8_t m_interruptNumber;
    InterruptHandler(uint8_t inum)
       :   m_interruptNumber(inum)
          
    {
        //InterruptManager::instance().interruptHandlers[inum] = this;
    }
    
    ~InterruptHandler()
    {
        
    }
    

private:
    virtual uint32_t handle(uint32_t esp){return esp;}
    //virtual void     diactivate();
    

};

class InterruptManager
{
    friend class InterruptHandler;
protected:
    struct GateDescriptor
    {
        uint16_t wHandlerAddressLowBits;
        uint16_t wCodeSegmentSelector;
        uint8_t  bReserved;
        uint8_t  bmAccess;
        uint16_t wHandlerAddressHighBits;
        
    } __attribute__((packed));
    
    
    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelector,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );
    
    void masterComplete();
    void slaveComplete();
    
public:
    
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();
    
    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;
    
public:
    
    void Activate();
    
    void addHandler(InterruptHandler* handler);
    
    
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void InterruptIgnore();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x0C();
    
public:
    static GateDescriptor interruptDescriptorTable[256];
    InterruptHandler* interruptHandlers[256];
    
    static InterruptManager* ActiveInterruptManager;
};



#endif
