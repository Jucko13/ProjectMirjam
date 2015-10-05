#ifndef LIGHT_H
#define LIGHT_H

class Light
{
    public:
        Light(int);
        virtual ~Light();
        void on();
        void off();
        void toggle();
        bool getStatus() const;
    private:
        bool status;
        int pin;
        void writeLamp(int pinA, bool status);
};

#endif // LIGHT_H
