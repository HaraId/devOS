#ifndef __DHARMA_PCI_H
#define __DHARMA_PCI_H

#include <hardwarecommunication/interrupts.h>
#include <tm/stdio.h>

/*
	bus_0 -> 
	          dev_0 -> 
						func_0
						...
						func_7
			  ...
			  dev_31
	...
	bus_7
*/

/*

Base Address Registers:

    	0x00 0x01 |  0x02 0x03 | 0x04 0x05 | 0x06 0x07  | 0x08    | 0x09 0x0A 0x0B | 0x0C       | 0x0D    |    0x0E     | 0x0F
0x00 | Vendor ID  |  Device ID | Cmd Reg   | Status reg | Revision| Class Code     | Cache line | L Timer | Header type | BIST

in case headertype == 0x00 (Standard)

0x10 |  Base Address 0         | Base Address 1         | Base Address 2           |  Base Address 3 
0x20 |  Base Address 4         | Base Address 5         | CardBus CIS pointer      | Subsys VID           | Subsys PID     
0x30 | Expansion Rom Base Addr | RESERVED                                          | IRQ line   | IRQ pin | Min_Gnt     | Max_Lat

in case headertype == 0x01 PCI/PCI-Bridge
.......


Base Address Reg types:
1) I/O registers      ..x1
2) Mem Mapping        .xx0      00 - 32bit; 01 - 20bit; 10 - 64bit memeory map 

*/

enum class PCIRegistersCode{
	VendorID = 0x00,
	DeviceID = 0x02,
	ClassID = 0x0B,
	Revision = 0x08,
	SubClassID = 0x0A,
	InterfaceID = 0x09,
	HeaderType = 0x0E,

	BaseAddr0 = 0x10,
	BaseAddr1 = 0x14,
	BaseAddr2 = 0x18,
	BaseAddr3 = 0x1C,
	BaseAddr4 = 0x20,
	BaseAddr5 = 0x24,

	SubSystemVID = 0x2C,
	SubSystemPID = 0x2E,

	ExpansionRomBaseAddr = 0x30,
	IRQline = 0x3C,
	IRQpin  = 0x3D,
	Min_Gnt = 0x3E,
	Max_Lat = 0x3F
};


using namespace tm;

enum class BaseAddressRegisterType
{
	MemoryMapping = 0,
	InputOutput = 1,
	NotSupported = 0xFF
};

class BaseAddressRegister
{
public:
	bool prefetchable;
	uint8_t *address;
	uint32_t size;						// only for MemoryMapping type
	BaseAddressRegisterType regType;
};

class PCIDeviceDescriptor
{
public:
	uint32_t portBase;
	uint32_t interrupt;

	uint16_t bus;
	uint16_t device;
	uint16_t function;

	uint16_t vendor_id;
	uint16_t device_id;
	
	uint16_t class_id;
	uint16_t subclass_id;
	uint16_t interface_id;

	uint16_t revision;

};



class PCIManager
{
public:
	PCIManager();
	~PCIManager();

	uint32_t Read(const uint16_t busn, 
		const uint16_t devicen, 
		const uint16_t funcn,
		const uint32_t regoffset);
	void Write(const uint16_t busn, 
		const uint16_t devicen, 
		const uint16_t funcn,
		const uint32_t regoffset,
		const uint32_t value);
	bool DeviceHasFunctions(const uint16_t busn, 
		const uint16_t devicen);

	PCIDeviceDescriptor ReadDeviceDescriptor(const uint16_t busn, 
		const uint16_t devicen, 
		const uint16_t funcn);

	void enumeration ();

	BaseAddressRegister getBaseAddressRegister(const uint16_t busn, 
		const uint16_t devicen, 
		const uint16_t funcn,
		uint16_t num);

private:
	Port32Bit dataPort;
	Port32Bit commandPort;

};


#endif