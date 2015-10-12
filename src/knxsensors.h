#ifndef KNXSENSORS_H
#define KNXSENSORS_H

class KnxSensors
{
    public:
        KnxSensors();
        virtual ~KnxSensors();
        void init();
        void setValues(int a, int b, int c);
    private:

};

#endif // KNXSENSORS_H
