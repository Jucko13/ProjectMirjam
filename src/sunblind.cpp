#include "sunblind.h"

Sunblind::Sunblind(int p): servo(p), status(false)
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
	
	servo.setAngle(a);
}

bool Sunblind::getStatus()
{
    return status;
}
