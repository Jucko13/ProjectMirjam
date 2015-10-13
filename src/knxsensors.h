#ifndef KNXSENSORS_H
#define KNXSENSORS_H

#include <string>

class KnxSensors
{
    public:
        KnxSensors();
        virtual ~KnxSensors();
        void init();
        void groupWrite(std::string a, std::string b);
    private:

};

#endif // KNXSENSORS_H
