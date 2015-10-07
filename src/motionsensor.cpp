#include "motionsensor.h"

<<<<<<< HEAD
#include <iostream>

MotionSensor::MotionSensor(int a) : pin(a)
{
	pinMode(a, INPUT);
=======
motionsensor::motionsensor(int a) : pin(a), status(false)
{
    pinMode(a, INPUT);
    attachInterrupt(a,setStatus,RISING);
>>>>>>> origin/master
}

motionsensor::~motionsensor()
{
    //dtor
}

void motionsensor::initMotion()
{
    digitalWrite(pin, LOW);
    for(int i = 0; i < calibrationTime; i++){
        delay(1000);
    }
}

void motionsensor::setStatus()
{
    status = true;
    timeTicks = 0;
}

<<<<<<< HEAD
bool MotionSensor::getStatus() {
    int value = digitalRead(pin);
    std::cout<<value << "\r\n";
    if(value < 1000)
        return true;
    return false;
=======
bool motionsensor::getStatus() const
{
    return status;
>>>>>>> origin/master
}

void motionsensor::timeCheck()
{
    if(status == true && digitalRead(pin) == LOW){
        timeTicks++;

        if (timeTicks > 50){
            status = false;
        }
    }
}
