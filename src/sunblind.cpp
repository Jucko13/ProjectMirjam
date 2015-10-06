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

bool Sunblind::getStatus()
{
    return status;
}
