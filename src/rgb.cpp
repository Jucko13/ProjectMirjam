#include "rgb.h"

RGB::RGB()
{
    kleuren[0] = 0;
    kleuren[1] = 0;
    kleuren[2] = 0;
    w = 0;
    status = 0;
    //ctor
}

RGB::~RGB()
{
    //dtor
}

void RGB::setColor(unsigned char a, unsigned char b)
{
    kleuren[a] = b;
}

void RGB::setWhite(bool a)
{
    if (a) {
        kleuren[0] = 255;
        kleuren[1] = 255;
        kleuren[2] = 255;
        w = 1;
    } else {
        kleuren[0] = 0;
        kleuren[1] = 0;
        kleuren[2] = 0;
        w = 0;
    }

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


unsigned char RGB::getColor(unsigned char a) const
{
    return kleuren[a];
}
