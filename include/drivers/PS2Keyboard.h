#ifndef __DHARMA_KEYBOARD_H
#define __DHARMA_KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

#include <drivers/driver.h>

#include <tm/stdio.h>

/*
 IBM PS/2 (i8042) 
 Эта клавиатура поддерживает второй и третий набор команд, однако контроллер клавиатуры способен транслировать наборы 2, 3 в набор 1
 * Порты контроллера клавиатуры:
    0x60 - data port
    0x64 - command port
    
    Ex:
    in al, 0x60 - read 
    out 0x60, al
    
    Command for keyboard controller:
       Name              | Code    | Args           | Return
    1) ReadCommandByte   | 0x20    | -------------- | command byte
    2) WriteCommandByte  | 0x60    | command byte   | -------------
    3) KeyBoardInter.Test| 0xAB    | -------------- | 0x00 - OK
    4) ControllerSelfTest| 0xAA    | -------------- | 0x55 - OK
    
    what'a command byte:
    
    +---+----+---+---+---+---+----+---+
    |7  |6   |5  |4  |3  |2  |1   |0  |
    +---+----+---+---+---+---+----+---+
    |...|XLAT|EN2|EN |...|SYS|int2|int|
    +---+----+---+---+---+---+----+---+
    
    XLAT - translate scan code to set 1 (1 - enable)
    INT2 - interrupt from mouse
    INT  - interrupt from key  board 0x01 (1 - to enable)
    
    Commands for keyboard self:
    для того чтобы отправить команду непосредственно клавиатуре, тоесть заставить контроллер клавиатуры
    переслать данные ей, необходимо писать в порт 0х60, аргументы передаются тудаже.
    Результат: 0xFA - OK, 0xFE - Resend
    
       Name            | code  | arg         | result
    1) RESEND          | 0xFE  | ---         | last sended byte or 0xFE(Resend)
    2) DISABLE         | 0xF5  | ---         | 0xFA
    3) ENABLE          | 0xF4  | ---         | 0xFA
    4) SET_SCAN_CODE   | 0xF0  | 0x01 or 0x03| 0xFA
    5) Echo            | 0xEE  | ---         | 0xEE
    6) Ser/Reset Leds  | 0xED  | bm          | 0xFA
   
   
   
Scan code set 1:
    Key Ex |  make        |  break
    A         0x1E            0x9E = make AND 0x80
    Special:
    Insert    0xE0 0x52       0xE0 0xD2 = make AND 0x80
    
Scan code set 2:
    Key Ex |  make        | break
    A         0x1C          0xF0 0x1C
    Insert    0xE0 ox70     0xE0 0xF0 0x70
    
Scan code set 3: 
    Key Ex |  make        | break
    A         0x1C          0xF0 0x1C
    Insert    0x67          0xF0 0x67
    Pouse     0x62          0xF0 0x62
    
 * 
 * */

class PS2KeyboardDriver : public InterruptHandler, public Driver
{
public:
    PS2KeyboardDriver();
    ~PS2KeyboardDriver();
    
// InterruptHandler interface:
    uint32_t handle(uint32_t esp) override;
    

// Driver interface:
    virtual void Activate() override;
    virtual int Reset() override;
    virtual void Deactivate() override;
    
private:
    Port8Bit m_dataPort;
    Port8Bit m_commandPort; 
};




#endif
