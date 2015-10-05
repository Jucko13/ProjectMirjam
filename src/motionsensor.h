#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H


class MotionSensor
{
public:
	MotionSensor(int a);
	virtual ~MotionSensor();
	bool getStatus();


private:
    int pin;


};

#endif // MOTIONSENSOR_H
