#include "hardwarecommunication/stdio.h"
#include "hardwarecommunication/gdt.h"
#include "hardwarecommunication/interrupts.h"

#include "drivers/driver.h"

#include "drivers/PS2Keyboard.h"
#include "drivers/PS2Mouse.h"
 
 
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}


extern "C" void kernelMain(void* multiboot_struction, uint32_t magicnumber)
{
    tm::resetTextSubSystem();
    GlobalDescriptorTable gdt;
    InterruptManager interruptManager(&gdt);
    DriverManager driverManager;
    
    uint32_t status = 0;
    

    tm::printString("Hello World from grub!\n");
    tm::printString("It's my first os!!!\n");
    
    tm::printString("Init interrupts...\n");
    


    
    PS2KeyboardDriver keyboard;
    PS2MouseDriver mouse;
    
    interruptManager.addHandler((InterruptHandler*)&keyboard);
    interruptManager.addHandler((InterruptHandler*)&mouse);

    driverManager.AddDriver(&keyboard);
    driverManager.AddDriver(&mouse);
    
    interruptManager.Activate();
    driverManager.ActivateAll();
    
    
    while( 1 )
    {
        //printf("$");
        //input();
        
    }
}









