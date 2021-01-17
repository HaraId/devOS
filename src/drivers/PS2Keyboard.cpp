
#include <drivers/PS2Keyboard.h>

PS2KeyboardDriver::PS2KeyboardDriver()
    : InterruptHandler(0x21),
      m_dataPort(0x60),
      m_commandPort(0x64)
    
{
    
}

PS2KeyboardDriver::~PS2KeyboardDriver(){}


/* virtual */ void PS2KeyboardDriver::Activate() 
{
    while(m_commandPort.Read() & 0x1)
        m_dataPort.Read();
    
    m_dataPort.Write(0xEE);
    if ( m_dataPort.Read() != 0xEE )
        return;
    
    m_dataPort.Write(0xF0); // command for keyboard - select scan code set
    m_dataPort.Write(0x03); // arg: scan code set 3
    if ( m_dataPort.Read() != 0xFA ) 
        return;
    
    
    m_commandPort.Write(0xae); // activate interrupts
    
    m_commandPort.Write(0x20); // Read command byte
    uint8_t status = (m_dataPort.Read() | 1) & ~(0x10 | 0x40); // Enable INT and Disable EN, XLAT bits.
    
    m_commandPort.Write(0x60); // Write command byte 
    m_dataPort.Write(status);  // Write arg 1 for prev command
    
    
    m_dataPort.Write(0xf4); // command for keyboard: activate
    
    
}

/* virtual */ int PS2KeyboardDriver::Reset() 
{
    return 0;
}

/* virtual */ void PS2KeyboardDriver::Deactivate()
{

} 


/* virtual */ uint32_t PS2KeyboardDriver::handle(uint32_t esp)
{
    uint8_t key = m_dataPort.Read();
    
    tm::printString("$KeyboardInterrupt\n");
    
    if ( key < 0x80 )
    {
        switch(key)
        {
            case 0x02: tm::printString("pressed[1]");break;
            default: tm::printString("pressed...");break;
        }
        
        
    }
    return esp;
}

