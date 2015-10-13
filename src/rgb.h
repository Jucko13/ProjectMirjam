#ifndef RGB_H
#define RGB_H

#include <string>
#include "knxsensors.h"

class RGB
{
    public:
        RGB(KnxSensors * k);
        virtual ~RGB();

        /*void setRed(char);
        void setGreen(char);
        void setBlue(char);*/
        void setColor(int a, std::string b);
        void setColorString(std::string, std::string, std::string);

        void on();
        void off();

        bool getStatus() const;
        std::string getColor(int) const;
    private:
        std::string kleuren[3];
        int w;
        bool status;
        KnxSensors *knx;
};

#endif // RGB_H
