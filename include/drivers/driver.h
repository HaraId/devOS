#ifndef __DHARMA_DRIVER_H
#define __DHARMA_DRIVER_H

#include <common/types.h>


class Driver
{
public:
	Driver();
	~Driver();
	
	virtual void Activate();
	virtual int Reset();
	virtual void Deactivate();
};

class DriverManager
{
private:
	Driver* drivers[256];
	int driversNum;

public:
	DriverManager();

	void ActivateAll();
	void AddDriver(Driver* driver);

};


#endif