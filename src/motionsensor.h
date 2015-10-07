#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "arduPi.h"


class motionsensor
{
    public:
        motionsensor(int a);
        virtual ~motionsensor();
        void initMotion();
        bool getStatus() const;
        void setStatus();
        void timeCheck();
    private:
        int pin, calibrationTime = 30, timeTicks = 0;
        bool status;

};

#endif // MOTIONSENSOR_H
