#include <drivers/PS2Mouse.h>


PS2MouseDriver::PS2MouseDriver()
    : InterruptHandler(0x2C),
      m_dataPort(0x60),
      m_commandPort(0x64)
    
{
    m_off = 0;
    m_buttonsFlag = 0;
}

PS2MouseDriver::~PS2MouseDriver(){}

/* virtual */ void PS2MouseDriver::Activate() 
{
    m_commandPort.Write(0xA8); // activate interrupts
    m_commandPort.Write(0x20); // get current state
    uint8_t status = m_dataPort.Read() | 0x02;
    
    m_commandPort.Write(0x60); // set status
    m_dataPort.Write(status);  // Write arg 1 for prev command
    
    m_commandPort.Write(0xD4);
    m_dataPort.Write(0xF4); // 
    m_dataPort.Read();
}

/* virtual */ int PS2MouseDriver::Reset() 
{
    return 0;
}

/* virtual */ void PS2MouseDriver::Deactivate()
{

} 


/* virtual */ uint32_t PS2MouseDriver::handle(uint32_t esp)
{

    uint8_t status = m_commandPort.Read();
    if ( !(status & 0x20) ) // isn't data
        return esp;
    
    static int8_t x = 0, y = 0;
    
    m_buff[m_off] = m_dataPort.Read();
    m_off = (m_off + 1) % 3;
    
    if ( m_off == 0 )
    {                                         /// @todo
        /*m_buttonsFlag = m_buff[0];
        x += m_buff[1];
        y += m_buff[2];*/
        
        //tm::printByte(m_buff[1]);

        if ( m_buff[1] == 0x09 )
            tm::scrollUp(1);
        else if ( m_buff[1] == 0x0A )
            tm::scrollDown(1);
    }
    
    return esp;
}

