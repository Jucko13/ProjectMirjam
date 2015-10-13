#include "rgb.h"

RGB::RGB(KnxSensors *k)
{
    kleuren[0] = "0";
    kleuren[1] = "0";
    kleuren[2] = "0";
    w = 0;
    status = 0;
    knx = k;
    //ctor
}

RGB::~RGB()
{
    //dtor
}

void RGB::setColor(int a, std::string b)
{
    kleuren[a] = b;
}

void RGB::setColorString(std::string r, std::string g, std::string b)
{
	kleuren[0] = r;
	kleuren[1] = g;
	kleuren[2] = b;
	
	knx->groupWrite("1/2/4", r + " " + g + " " + b);

}

void RGB::on()
{
    status = true;
}

void RGB::off()
{
    status = false;
}

bool RGB::getStatus() const
{
    return status;
}


std::string RGB::getColor(int a) const
{
    return kleuren[a];
}
