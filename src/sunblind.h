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
		void setPosition(int a);
        bool getStatus();
        void toggle();
        int getPosition();
        
    private:
        bool status;
        int pin;
        int pos;
        Servo servo;

};

#endif // DOOR_H
