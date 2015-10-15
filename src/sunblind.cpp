#include "sunblind.h"

Sunblind::Sunblind(int p): servo(p), status(false), pos(0)
{

}

Sunblind::~Sunblind()
{

}
/**
 * @brief Sunblind::open sets the status of the sunblind to true, then calls Servo::setAngle() to change the angle of the attached servo to it's maximum which is 90.
 */
void Sunblind::open()
{
    status = true;
    servo.setAngle(180);
}

/**
 * @brief Sunblind::close sets the status of the sunblind to false, then calls Servo::setAngle() to change the angle of the attached servo to it's minimum which is 0.
 */
void Sunblind::close()
{
    status = false;
    servo.setAngle(0);
}

/**
 * @brief Sunblind::toggle checks the status of the sunblind. If the status is false, Sunblind::open() is called, if the status is true, Sunblind::close() is called.
 */
void Sunblind::toggle(){
	status = (status ? false : true);
	if(status)
		open();
	else
		close();
}

/**
 * @brief Sunblind::setPosition sets status and the angle of the servo in degrees. If the angle is 0, the status is set to false. In all other cases, status is true.
 * @param a is the integer value in degrees for the position to which the sunblind should be set. This value can be between 0 and 90.
 */
void Sunblind::setPosition(int a)
{
	if(!a)
		status = false;
	else
		status = true;
	
	pos = a;
	
	servo.setAngle(a);
}

/**
 * @brief Sunblind::getPosition gets the angle which is set on the servo.
 * @return the integer value of the position of the servo in degrees.
 */
int Sunblind::getPosition()
{
	return pos;
}

/**
 * @brief Sunblind::getStatus gets the status of the sunblind. This status is only false when the sunblind is fully retracted and at position 0.
 * @return a boolean status represing whether or not the sunblind is extended.
 */
bool Sunblind::getStatus()
{
    return status;
}
