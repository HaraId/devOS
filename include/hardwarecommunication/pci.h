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

using namespace tm;

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

	void enumeration();

private:
	Port32Bit dataPort;
	Port32Bit commandPort;

};


#endif