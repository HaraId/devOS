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
	PCIDeviceDescriptor pci_device;
	BaseAddressRegister bar;

	uint32_t busNum;
	uint32_t deviceNum;
	uint32_t funcNum;
	uint32_t barNum;


	for (busNum = 0; busNum < 8; ++busNum)
	{
		for (deviceNum = 0; deviceNum < 32; ++deviceNum)
		{

			const int funcCount = DeviceHasFunctions(busNum, deviceNum) ? 8 : 1;

			for (funcNum = 0; funcNum < funcCount; ++funcNum)
			{
				pci_device = ReadDeviceDescriptor(busNum, deviceNum, funcNum);

				if ( pci_device.vendor_id == 0x0000 || pci_device.vendor_id == 0xFFFF )
					continue;

				printString("PCI[ ");
				printByte(busNum);
				printString(" | ");
				printByte(deviceNum);
				printString(" | ");
				printByte(funcNum);
				printString(" ] vid: ");

				printWord(pci_device.vendor_id);

				printString(" pid: ");
				printWord(pci_device.device_id);

				printString(" class: ");
				printWord(pci_device.class_id);
				printString(" subclass: ");
				printWord(pci_device.subclass_id);
				printString("\n");

				printString("       ");

				for (barNum = 0; barNum < 6; barNum++)
				{	
					bar = getBaseAddressRegister(busNum, deviceNum, funcNum, barNum);

					if ( bar.address && (bar.regType == BaseAddressRegisterType::InputOutput) )
						pci_device.portBase = (uint32_t) bar.address;

					printByte((uint8_t)bar.regType);
					printString(" ");
				}

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

	desc.vendor_id = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::VendorID));
	desc.device_id = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::DeviceID));
	
	desc.class_id = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::ClassID));
	desc.subclass_id = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::SubClassID));
	desc.interface_id = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::InterfaceID));

	desc.revision = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::Revision));
	desc.interrupt = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::IRQline));

	return desc;
}

BaseAddressRegister PCIManager::getBaseAddressRegister(const uint16_t busn, const uint16_t devicen, const uint16_t funcn, uint16_t num)
{
	BaseAddressRegister bar;

	uint32_t headertype = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::HeaderType)) & 0x7F;
	uint32_t maxBARs  = 6 - (4 * headertype);

	if ( num > maxBARs )
	{
		bar.regType = BaseAddressRegisterType::NotSupported;
		return bar;
	}

	uint32_t bar_value = Read(busn, devicen, funcn, uint8_t(PCIRegistersCode::BaseAddr0) + 4 * num);
	bar.regType = (bar_value & 0x01) ? BaseAddressRegisterType::InputOutput : BaseAddressRegisterType::MemoryMapping;
	

	if ( bar.regType == BaseAddressRegisterType::MemoryMapping )
	{
		switch( (bar_value >> 1) & 0x03 )
		{
			case 0b00: //32 bit mode
			break;
			case 0b01: //20 bit mode
			break;
			case 0b10:
			break;
		}

		bar.prefetchable = ((bar_value >> 3) & 0x01) == 0x01;
	}
	else
	{
		bar.address = (uint8_t*)(bar_value & ~0x03);
		bar.prefetchable = false;
	}

	return bar;
}