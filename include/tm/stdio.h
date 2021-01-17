#ifndef __DHARMA_STDIO_H
#define __DHARMA_STDIO_H


#include <common/types.h>
#include <common/stdlib.h>

/*
 *  80 x 25
 * 
 *  text mode:
 *  
 *  +--+--------+----------+---------------+
 *  |15|14    12|11       8|7             0|
 *  +--+--------+----------+---------------+
 *  | ?|bg color|text color|  ascii        |
 *  +--+--------+----------+---------------+
 * */

namespace tm
{


static int16_t STDIO_STDCOLOR_ATTR = 0x0F00;
static int8_t STDIO_POS_X = 0;
static int8_t STDIO_POS_Y = 0;

static const uint16_t SCREEN_BUFF_WIDTH = 80;
static const uint16_t SCREEN_BUFF_HEIGHT = 25;

static const uint16_t TEXT_BUFF_WIDTH = 80;
static const uint16_t TEXT_BUFF_HEIGHT = 200;


static uint16_t textplain_buffer [TEXT_BUFF_HEIGHT][TEXT_BUFF_WIDTH];
static int16_t buffer_offset = 0;


void resetTextSubSystem();
void clearScreen();
void clearLine();
inline uint16_t color_std(uint8_t ascii = ' ');


void flushBufferInScreen();
void scrollUp(uint16_t value);
void scrollDown(uint16_t value);
void scrollSeek(uint16_t pos);

void putchar(char value);

void printString(const char* str);
void printByte(const uint8_t value);
void printWord(const uint16_t value);
void printDWord(const uint32_t value);
void printAddr(const void* addr);




int8_t moveCursor(const uint8_t x, const uint8_t y);
//void setAttribute(const char uint8_t attr); 

}





#endif
