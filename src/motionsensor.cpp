#include "motionsensor.h"
#include "arduPi.h"

#include <iostream>

MotionSensor::MotionSensor(int a) : pin(a)
{
	pinMode(a, INPUT);
}

MotionSensor::~MotionSensor() {

}


bool MotionSensor::getStatus() {
    int value = digitalRead(pin);
    std::cout<<value << "\r\n";
    if(value < 1000)
        return true;
    return false;
}


