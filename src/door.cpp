#include "door.h"
#include "arduPi.h"


Door::Door(int p): pin(p), status(false)
{
	pinMode(p,OUTPUT);
}

Door::~Door()
{ 
	//dtor
}

void Door::open()
{
	status = true;
	setAngle(1500);
}

void Door::setAngle(int angleDelay){
	for(int i = 0; i<20; i++){
		digitalWrite(pin,HIGH);
		delayMicroseconds(angleDelay);
		digitalWrite(pin,LOW);
		delayMicroseconds(20000-angleDelay);
	}
}

void Door::close()
{
	status = false;
	setAngle(750);
}

bool Door::getStatus()
{
	return status;
}

