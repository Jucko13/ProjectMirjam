#include "sunblind.h"

Sunblind::Sunblind(int p): servo(p), status(false), pos(0)
{

}

Sunblind::~Sunblind()
{

}

void Sunblind::open()
{
    status = true;
    servo.setAngle(90);
}

void Sunblind::close()
{
    status = false;
    servo.setAngle(0);
}

void Sunblind::toggle(){
	status = (status ? false : true);
	if(status)
		open();
	else
		close();
}

void Sunblind::setPosition(int a)
{
	if(!a)
		status = false;
	else
		status = true;
	
	pos = a;
	
	servo.setAngle(a);
}

int Sunblind::getPosition()
{
	return pos;
}

bool Sunblind::getStatus()
{
    return status;
}
