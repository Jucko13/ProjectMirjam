#ifndef DOOR_H
#define DOOR_H

#include <pthread.h>
#include <iostream>

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
        int pin;
        void setAngle(int angleDelay);
			
        /*void * ServoThread(void * dummyptr);
        pthread_t servo;*/
};

#endif // DOOR_H
