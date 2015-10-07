#include "motionsensor.h"

motionsensor::motionsensor(int a) : pin(a), status(false)
{
    pinMode(a, INPUT);
    attachInterrupt(a,setStatus,RISING);
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

bool motionsensor::getStatus() const
{
    return status;
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
