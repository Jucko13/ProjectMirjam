#include "rgb.h"

RGB::RGB(KnxSensors *k)
{
    kleuren[0] = "0";
    kleuren[1] = "0";
    kleuren[2] = "0";
    kleuren[3] = "0";
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

void RGB::setColorString(std::string r, std::string g, std::string b, std::string w)
{
	kleuren[0] = r;
	kleuren[1] = g;
	kleuren[2] = b;
	kleuren[3] = w;
	
	knx->groupWrite("1/2/4", r + " " + g + " " + b);
	knx->groupWrite("1/2/3", w);
}


std::string RGB::getColor(int a) const
{
    return kleuren[a];
}
