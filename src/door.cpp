#include "door.h"

Door::Door(int p): servo(p), status(false)
{
}

Door::~Door()
{ 
	//dtor
}

void Door::open()
{
	status = true;
	servo.setAngle(90);
}

void Door::close()
{
	status = false;
	setAngle(0);
}

bool Door::getStatus()
{
	return status;
}

