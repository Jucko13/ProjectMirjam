#include "motionsensor.h"

MotionSensor::MotionSensor(int a) : pin(a)
{

}

MotionSensor::~MotionSensor() {

}


bool MotionSensor::getStatus() {
    int value = analogRead(pin);
    if(value < 1000)
        return true;
    return false;
}


