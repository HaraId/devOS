#ifndef __DHARMA_GDT_H
#define __DHARMA_GDT_H

#include "common/types.h"

/*
 * 
 * 
 * GDT:
 * 
 * lgdt - save global descriptor GlobalDescriptorTable
 * sgdt - recive GlobalDescriptorTable
 * 
 * rgdt - register
 *      format:
 *  +-------------------+------+
 *  |47               16|15   0|
 *  +-------------------+------+
 *  | Table Address     |Limit |
 *  +-------------------+------+
 * 
 * SELECTOR:
 *  +-----------------------------------+-------------------+--+----+
 *  |79                               16|15                3|2 |1  0|
 *  +-----------------------------------+-------------------+--+----+
 *  |      GDT                          | Table Address     |TI|RPL |
 *  +-----------------------------------+-------------------+-------+
 * 
 * Ex:
 * mov fword [GDT]
 * 
 * 
 
 +-----------+--+--+-----+----------+--+-----+--+------+-------------+
 |31       24|23|22|21|20|19      16|15|14,13|12|11   8|7           0|
 +-----------+--+--+--+--+----------+--+-----+--+------+-------------+
 |Base Addr  |G |D |L |AV|Limit bits|P | DPL |S |type  | Base Addr   |
 | 24 - 31   |  |B |  |-L|  16 - 19 |  |     |  |      | 16 - 23     |
 +-----------+--+--+--+--+----------+--+-----+--+------+-------------+
 |31                              16|15                             0|
 +----------------------------------+--------------------------------+
 |  Base Addr 0 - 15                |  Limit segment 0 - 15          |
 +----------------------------------+--------------------------------+
 * 
 * */

typedef struct _RGDT_STRUCT
{
    uint32_t  tableLimit;
    uint32_t tableAddress;
}  RGDT_STRUCT, *PRGDT_STRUCT;


class GlobalDescriptorTable
{
public:
    class SegmentDescriptor
    {
    private:
        uint16_t wLimitL;
        uint16_t wBaseL;
        
        uint8_t  bBaseHL;
        uint16_t wFlags;
        uint8_t  bBaseHR;
        
    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
        
        uint32_t Base();
        uint32_t Limit();
        
        
    } __attribute__((packed));
    
    
    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
    
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();
    
    uint16_t getCodeSegmentSelector();
    uint16_t getDataSegmentSelector();
    
};












#endif
