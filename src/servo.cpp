#include "servo.h"
#include "arduPi.h"

Servo::Servo(int a): pin(a)
{
    pinMode(a, OUTPUT);
}

Servo::~Servo()
{

}

/**
 * @brief Servo::setAngle sets the angle of the servo in degrees.
 * @param pos is the integer value for the angle to which the servo should turn in degrees.
 */
void Servo::setAngle(int pos)
{
    float angleDelay;
	
    if(!(pos >= 0 && pos <= 180)) return;
	
	angleDelay = 750 + ((1500/180)*pos);

    for(int i = 0; i<30; i++){
        digitalWrite(pin, HIGH);
        delayMicroseconds(angleDelay);
        digitalWrite(pin, LOW);
        delayMicroseconds(20000-angleDelay);
    }
}
