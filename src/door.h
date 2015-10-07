#ifndef DOOR_H
#define DOOR_H

#include <pthread.h>
#include <iostream>
#include "servo.h"

class Door
{
    public:
        Door(int p);
        virtual ~Door();
        void open();
        void close();
        bool getStatus();
    private:
        bool status;
		Servo servo;
        /*void * ServoThread(void * dummyptr);
        pthread_t servo;*/
};

#endif // DOOR_H
