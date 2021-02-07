#include <hardwarecommunication/pci.h>

PCIManager::PCIManager()
	: dataPort(0xCFC),
	  commandPort(0xCF8)
{

}

PCIManager::~PCIManager()
{

}


uint32_t PCIManager::Read(const uint16_t busn, const uint16_t devicen, const uint16_t funcn, const uint32_t regoffset)
{
	const uint32_t id = (0x01 << 31) | ((busn & 0xFF) << 16) | ((devicen & 0x1F) << 11) | ((funcn & 0x07) << 8) | (regoffset & 0xFC);

	commandPort.Write(id);

	return dataPort.Read() >> ( 8 * (regoffset % 4));
}

void PCIManager::Write(const uint16_t busn, const uint16_t devicen, const uint16_t funcn, const uint32_t regoffset, const uint32_t value)
{
	const uint32_t id = (0x01 << 31) | ((busn & 0xFF) << 16) | ((devicen & 0x1F) << 11) | ((funcn & 0x07) << 8) | (regoffset & 0xFC);
    
    // todo
}

bool PCIManager::DeviceHasFunctions(const uint16_t busn, const uint16_t devicen)
{
	return this->Read(busn, devicen, 0, 0x0E) & (1 << 7); 
}

void PCIManager::enumeration()
{

	for (int busI = 0; busI < 8; ++busI)
	{
		for (int deviceI = 0; deviceI < 32; ++deviceI)
		{
			int numFunc = DeviceHasFunctions(busI, deviceI) ? 8 : 1;
			for (int funcI = 0; funcI < numFunc; ++funcI)
			{
				PCIDeviceDescriptor pci_device = ReadDeviceDescriptor(busI, deviceI, funcI);

				if ( pci_device.vendor_id == 0x0000 || pci_device.vendor_id == 0xFFFF )
					break;

				printString("PCI[ ");
				printByte(busI);
				printString(" | ");
				printByte(deviceI);
				printString(" | ");
				printByte(funcI);
				printString(" ] vid: ");

				printWord(pci_device.vendor_id);

				printString(" pid: ");
				printWord(pci_device.device_id);
				printString("\n");
			}

		}
	}
}

PCIDeviceDescriptor PCIManager::ReadDeviceDescriptor(const uint16_t busn, const uint16_t devicen, const uint16_t funcn)
{
	PCIDeviceDescriptor desc;

	desc.bus = busn;
	desc.device = devicen;
	desc.function = funcn;

	desc.vendor_id = Read(busn, devicen, funcn, 0x00);
	desc.device_id = Read(busn, devicen, funcn, 0x02);
	
	desc.class_id = Read(busn, devicen, funcn, 0x0b);
	desc.subclass_id = Read(busn, devicen, funcn, 0x0a);
	desc.interface_id = Read(busn, devicen, funcn, 0x09);

	desc.revision = Read(busn, devicen, funcn, 0x08);
	desc.interrupt = Read(busn, devicen, funcn, 0x3c);

	return desc;
}
