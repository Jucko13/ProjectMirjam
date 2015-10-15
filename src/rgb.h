#ifndef RGB_H
#define RGB_H

#include <string>
#include "knxsensors.h"

class RGB
{
    public:
        RGB(KnxSensors * k);
        virtual ~RGB();

        void setColor(int a, std::string b);
        void setColorString(std::string, std::string, std::string, std::string w);

        std::string getColor(int) const;
    private:
        std::string kleuren[4];
        KnxSensors *knx;
};

#endif // RGB_H
