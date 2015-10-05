#include "light.h"
#include "arduPi.h"
#include <iostream>

void Light::writeLamp(int pinA, bool status)
{
    if (status)
        digitalWrite(pinA, HIGH);
    else
        digitalWrite(pinA, LOW);
}

Light::Light(int a) : pin(a)
{
	status = false;
	pinMode(a,OUTPUT);
    //ctor
}

Light::~Light()
{
    //dtor
}

void Light::on()
{
    status = true;
    writeLamp(pin, status);
}

void Light::off()
{
    status = false;
    writeLamp(pin, status);
}

void Light::toggle()
{
    status = (status ? false : true);
    writeLamp(pin, status);
}

bool Light::getStatus() const
{
    return status;
}
