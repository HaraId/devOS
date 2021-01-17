#include "hardwarecommunication/interrupts.h"
#include "tm/stdio.h"



InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256] = { 0 };
//InterruptHandler* InterruptManager::interruptHandlers[256] = { 0 };

InterruptManager* InterruptManager::ActiveInterruptManager = nullptr;


void InterruptManager::Activate()
{   

    asm("sti");
}

    
void InterruptManager::SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelector,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    )
{
    const uint8_t IDT_DESC_PRESENT = 0x80;
    
    interruptDescriptorTable[interruptNumber].wHandlerAddressLowBits = ((int32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].wHandlerAddressHighBits = (((int32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].wCodeSegmentSelector = codeSegmentSelector;
    interruptDescriptorTable[interruptNumber].bmAccess = IDT_DESC_PRESENT | (DescriptorType & 0x1F) | ((DescriptorPrivilegeLevel << 5) & 0x60);
    interruptDescriptorTable[interruptNumber].bReserved = 0x00;
}
    
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
    : picMasterCommand(0x20),
      picMasterData(0x21),
      picSlaveCommand(0xA0),
      picSlaveData(0xA1)
    
{
    if (ActiveInterruptManager == nullptr)
    {
        ActiveInterruptManager = this;
    }
    
    uint16_t CodeSegment = gdt->getCodeSegmentSelector();
    
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    
    for (uint16_t i = 0; i < 0xFF; ++i)
    {
        SetInterruptDescriptorTableEntry(
            i,
            CodeSegment,
            &InterruptIgnore,
            0,
            IDT_INTERRUPT_GATE
        );
        
        interruptHandlers[i] = nullptr;
    }

    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);
    
    // pribavit 20|28
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);
    
    
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);
    
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);
    
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    
    RIDT_STRUCT idt;
    idt.tableAddress = (uint32_t)interruptDescriptorTable;
    idt.tableLimit   = 256 * sizeof(GateDescriptor) - 1;
    
    asm volatile ("lidt %0" : : "m" (idt));
}

void InterruptManager::addHandler(InterruptHandler* handler)
{
    interruptHandlers[handler->m_interruptNumber] = handler;
}

InterruptManager::~InterruptManager()
{
    
}

void InterruptManager::masterComplete()
{
    picMasterCommand.Write(0x20);
}

void InterruptManager::slaveComplete()
{
    picSlaveCommand.Write(0x20);
}


uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    InterruptManager* im = InterruptManager::ActiveInterruptManager;
    
    if ( im == nullptr )
        return esp;
    
    if ( im->interruptHandlers[interruptNumber] != nullptr )
    {
        im->interruptHandlers[interruptNumber]->handle(esp);
    }else  if (interruptNumber != 0x20)// undefined interrupt
    {
        tm::printString("Info::InterruptManager catched undefined interruption [");
        tm::printByte(interruptNumber);
        tm::printString("]\n");
    }
    
    if ( 0x20 <= interruptNumber )
        {
            InterruptManager::ActiveInterruptManager->masterComplete();
            if ( 0x28 <=  interruptNumber )
                InterruptManager::ActiveInterruptManager->slaveComplete();
        }
    
    return esp;
}


