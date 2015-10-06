#ifndef SUNBLIND_H
#define SUNBLIND_H

#include <pthread.h>
#include <iostream>
#include "servo.h"

class Sunblind
{
    public:
        Sunblind(int p);
        virtual ~Sunblind();
        void open();
        void close();
        bool getStatus();
    private:
        bool status;
        int pin;
        Servo servo;

};

#endif // DOOR_H