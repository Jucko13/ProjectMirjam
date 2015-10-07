#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "arduPi.h"


class MotionSensor
{
    public:
        MotionSensor(int a,void (*b)());
        virtual ~MotionSensor();
        bool getStatus() const;
        void setStatus();
        void timeCheck();
    private:
        int pin, calibrationTime, timeTicks;
        bool status;

};

#endif // MOTIONSENSOR_H
