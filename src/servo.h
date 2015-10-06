#ifndef SERVO_H
#define SERVO_H
#include <iostream>


class Servo
{
public:
    Servo(int);
    ~Servo();
    void setAngle(int);
    int getAngle();
private:
    int pin;
    int position;
};

#endif // SERVO_H
