#include "light.h"
#include "arduPi.h"
#include <iostream>

Light::Light(int a) : pin(a)
{
	pinMode(a,INPUT);
	status = digitalRead(a);
	pinMode(a,OUTPUT);
    //ctor
}

Light::~Light()
{
    //dtor
}

/**
 * @brief Light::writeLamp uses arduPi library to set the given pin to the given status.
 * @param pinA selects the physical pin.
 * @param status selects the boolean status (either true or false) to set the pin to.
 */

void Light::writeLamp(int pinA, bool status)
{
    if (status)
        digitalWrite(pinA, HIGH);
    else
        digitalWrite(pinA, LOW);
}

/**
 * @brief Light::on sets status value to true and uses writeLamp() to set the physical pin on true.
 */

void Light::on()
{
    status = true;
    writeLamp(pin, status);
}

/**
 * @brief Light::off sets status value to false and uses writeLamp() to set the physical pin to false.
 */

void Light::off()
{
    status = false;
    writeLamp(pin, status);
}

/**
 * @brief Light::toggle toggles the status value between true or false and uses writeLamp() to set the physical pin to match the status value.
 */

void Light::toggle()
{
    status = (status ? false : true);
    writeLamp(pin, status);
}

/**
 * @brief Light::getStatus returns the saved status of lamp.
 * @return returns boolean variable, status, which can be true or false.
 */

bool Light::getStatus() const
{
    return status;
}
