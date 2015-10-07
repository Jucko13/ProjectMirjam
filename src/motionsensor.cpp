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

void MotionSensor::initMotion()
{
    digitalWrite(pin, LOW);
    for(int i = 0; i < calibrationTime; i++){
        delay(1000);
    }
}

void MotionSensor::setStatus()
{
    status = true;
    timeTicks = 0;
    std::cout<<"interrupt!!!" << "\r\n";
}

bool MotionSensor::getStatus() const
{
    return status;
}

void MotionSensor::timeCheck()
{
    if(status == true && digitalRead(pin) == LOW){
        timeTicks++;
		std::cout<<timeTicks << "\r\n";
        if (timeTicks > 50){
            status = false;
        }
    }
}
