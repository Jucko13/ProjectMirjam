#include "door.h"

Door::Door(int p): servo(p), status(false)
{
}

Door::~Door()
{ 
	//dtor
}

/**
 * @brief Door::open sets the openend value of the door to true and uses Servo.setAngle() to open the door.
 */
void Door::open()
{
	status = true;
	servo.setAngle(90);
}

/**
 * @brief Door::close sets the openend value of the door to false and uses Servo.setAngle() to close the door.
 */
void Door::close()
{
	status = false;
	servo.setAngle(0);
}

/**
 * @brief Door::getStatus checks the status whether or not the door is open and returns that.
 * @return a boolean status that is either true when the door is open or false when the door is closed.
 */
bool Door::getStatus()
{
	return status;
}

