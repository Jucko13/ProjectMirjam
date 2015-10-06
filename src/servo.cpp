#include "servo.h"
#include "arduPi.h"

Servo::Servo(int a): pin(a)
{
    pinMode(a, OUTPUT);
}

Servo::~Servo()
{

}

void Servo::setAngle(int pos)
{
    float angleDelay;
    if(!(pos >= 0 && pos <= 180))
        pos = getPosition();
    angleDelay = pos*5.55555555556+1000;
    for(int i = 0; i<20; i++){
        digitalWrite(pin, HIGH);
        delayMicroseconds(angleDelay);
        digitalWrite(pin, LOW);
        delayMicroseconds(20000-angleDelay);
        position = pos;
    }
}

int Servo::getAngle()
{
    return position;
}
