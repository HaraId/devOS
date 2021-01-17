#include "drivers/driver.h"


Driver::Driver()
{

}

Driver::~Driver()
{

}
	
/* virtual */ void Driver::Activate()
{

}


/* virtual */ int Driver::Reset()
{
	return 0;
}


/* virtual */ void Driver::Deactivate()
{

}


// ============================================= Driver Manager ========================================================

DriverManager::DriverManager()
{
	driversNum = 0;
}

void DriverManager::ActivateAll()
{
	for (uint16_t i = 0; i < driversNum; ++i)
		drivers[i]->Activate();
}

void DriverManager::AddDriver(Driver* driver)
{
	drivers[driversNum++] = driver;
}