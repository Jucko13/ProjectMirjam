#ifndef SERVO_H
#define SERVO_H
#include <iostream>


class Servo
{
public:
    Servo(int);
    ~Servo();
    void setAngle(int);
private:
    int pin;
};

#endif // SERVO_H
