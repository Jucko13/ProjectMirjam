#include "motionsensor.h"
#include <iostream>

MotionSensor::MotionSensor(int a, void (*b)()) : pin(a), status(false), calibrationTime(5), timeTicks(0)
{
    pinMode(a, INPUT);
    attachInterrupt(a,b,RISING);
	

	
}

MotionSensor::~MotionSensor()
{
    //dtor
}

void MotionSensor::setStatus()
{
    //std::cout<<"interrupt on pin " << pin <<"!\r\n";
    if(digitalRead(pin) == LOW) return;
    status = true;
    timeTicks = 0;
}

bool MotionSensor::getStatus() const
{
    return status;
}

void MotionSensor::timeCheck()
{
    if(status == true && digitalRead(pin) == LOW){
        timeTicks++;
		//std::cout<<timeTicks << "\r\n";
        if (timeTicks > 50){
            status = false;
        }
    }
}
