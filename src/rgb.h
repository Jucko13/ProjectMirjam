#ifndef RGB_H
#define RGB_H


class RGB
{
    public:
        RGB();
        virtual ~RGB();

        /*void setRed(char);
        void setGreen(char);
        void setBlue(char);*/
        void setWhite(bool);
        void setColor(unsigned char,unsigned char);

        void on();
        void off();

        bool getStatus() const;
        unsigned char getColor(unsigned char) const;
    private:
        unsigned char kleuren[3];
        int w;
        bool status;
};

#endif // RGB_H
