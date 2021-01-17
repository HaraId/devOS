#include "tm/stdio.h"

namespace tm{


void resetTextSubSystem()
{

    buffer_offset = 0;

    memsetw((uint16_t*)textplain_buffer, color_std(), TEXT_BUFF_WIDTH * TEXT_BUFF_HEIGHT);
    
    for (uint16_t x = 0; x < TEXT_BUFF_WIDTH; ++x)
        textplain_buffer[TEXT_BUFF_HEIGHT - 1][x] = color_std('=');
    
    scrollSeek( 0 );
}

uint16_t color_std(uint8_t ascii)
{
    return STDIO_STDCOLOR_ATTR | ascii;
}

void flushBufferInScreen()
{
    uint16_t* VideoMemory = (uint16_t*) 0xb8000;
    
    for (uint16_t y = 0; y < SCREEN_BUFF_HEIGHT; ++y)
    {
        const uint16_t linepos = (y + buffer_offset) % TEXT_BUFF_HEIGHT;
        
        for (uint16_t x = 0; x < SCREEN_BUFF_WIDTH; ++x)
        {
            *VideoMemory++ = textplain_buffer[linepos][x];
        }
    }
}

void scrollUp(uint16_t value)
{
    buffer_offset-= value;
    
    if ( buffer_offset < 0 )
        buffer_offset = TEXT_BUFF_HEIGHT - 1 + buffer_offset;
    
    flushBufferInScreen();
}
void scrollDown(uint16_t value)
{
    buffer_offset+= value;
    
    if ( buffer_offset > TEXT_BUFF_HEIGHT )
        buffer_offset = buffer_offset % TEXT_BUFF_HEIGHT;
    
    flushBufferInScreen();
}

void scrollSeek(uint16_t pos)
{
    buffer_offset = pos > TEXT_BUFF_HEIGHT ? 0 : pos;
    
    flushBufferInScreen();
}

void putchar(char value)
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;
    
    switch( value )
    {
        case '\n':
            STDIO_POS_Y ++;
            STDIO_POS_X = 0;
            return;
        break;
        case '\t':
            STDIO_POS_X += 3;
        break;
        
        default:
            STDIO_POS_X ++;
        
    }

    if ( STDIO_POS_X >= SCREEN_BUFF_WIDTH )
    {
        STDIO_POS_Y ++;
        STDIO_POS_X = 0;
    }
    
    if ( STDIO_POS_Y >= SCREEN_BUFF_HEIGHT )
    {
        STDIO_POS_Y = SCREEN_BUFF_HEIGHT - 1;
        scrollDown(1);
        clearLine();
    }
    
    VideoMemory[STDIO_POS_Y * SCREEN_BUFF_WIDTH + STDIO_POS_X] 
            = ( VideoMemory[STDIO_POS_Y * SCREEN_BUFF_WIDTH + STDIO_POS_X] & 0xFF00 ) | value;
    
    if ( 0 <= buffer_offset && buffer_offset < TEXT_BUFF_HEIGHT )
        textplain_buffer[(STDIO_POS_Y + buffer_offset) % TEXT_BUFF_HEIGHT][STDIO_POS_X] = VideoMemory[STDIO_POS_Y * SCREEN_BUFF_WIDTH + STDIO_POS_X];
    
}



void printString(const char* str)
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;
    
    
    for(int32_t i = 0; str[i] != '\0'; ++i)
    {
        putchar(str[i]);
    }
}

void printByte(const uint8_t value)
{
    const static char fbrk[] = "0123456789ABCDEF";

    putchar(fbrk[(value >> 4) & 0xF0]);
    putchar(fbrk[value & 0x0F]);
}

void printWord(const uint16_t value)
{
    printByte(value >> 8);
    printByte(value);
}

void printDWord(const uint32_t value)
{
    printByte(value >> 24);
    printByte(value >> 16);
    printByte(value >> 8);
    printByte(value);
}

void printAddr(const void* addr)
{
    printString("0x");
    uint32_t a = uint32_t(addr);

    printDWord(a);
}

void clearLine()
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    VideoMemory += STDIO_POS_Y * SCREEN_BUFF_WIDTH;

    memsetw(VideoMemory, color_std(), SCREEN_BUFF_WIDTH);
}   

void clearscreen()
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;
    
    for (uint16_t i = 0; i < SCREEN_BUFF_WIDTH * SCREEN_BUFF_HEIGHT; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | ' ';
}

int8_t moveCursor(const uint8_t x, const uint8_t y)
{
    if ( x >= SCREEN_BUFF_WIDTH )
        return 1;
    if ( y >= SCREEN_BUFF_HEIGHT )
        return 2;
    
    STDIO_POS_X = x;
    STDIO_POS_Y = y;
    
    return 0;
}

}