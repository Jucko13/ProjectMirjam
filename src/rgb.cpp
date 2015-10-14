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

/**
 * setColor uses an int to select the red, green or blue component of the light. It will then put a string into the the selected array slot. This string should be an integer between 0 and 255.
 * @brief RGB::setColor sets the values for individual colors in the rbg spectrum in an array.
 * @param a is between 0 and 2, 0 selects the red component, 1 selects the green component, 2 selects the blue component of the rgb spectrum.
 * @param b is a string that is between 0 and 255 to select the value of the in a defined component of rgb.
 */
void RGB::setColor(int a, std::string b)
{
    kleuren[a] = b;
}

/**
 * setColorString takes three strings to set the color of the rgb led attached to the KNX.
 * @brief RGB::setColorString requires 3 strings representing the values for red green and blue in the rgb spectrum. It will then call KnxSensors::groupWrite() to write this color to the KNX.
 * @param r is a string representation of the value for the red component of rgb. This value can be between 0 and 255.
 * @param g is a string representation of the value for the green component of rgb. This value can be between 0 and 255.
 * @param b is a string representation of the value for the blue component of rgb. This value can be between 0 and 255.
 */
void RGB::setColorString(std::string r, std::string g, std::string b)
{
	kleuren[0] = r;
	kleuren[1] = g;
	kleuren[2] = b;
	
	knx->groupWrite("1/2/4", r + " " + g + " " + b);
}

/**
 * @brief RGB::on turns the entire ledstrip on.
 */
void RGB::on()
{
    status = true;
}

/**
 * @brief RGB::off turns the entire ledstrip off.
 */
void RGB::off()
{
    status = false;
}

/**
 * @brief RGB::getStatus gets the status that allows the ledstrip to be on or off.
 * @return status which is a boolean that represents whether or not the ledstrip is on.
 */
bool RGB::getStatus() const
{
    return status;
}

/**
 * @brief RGB::getColor returns the value of either R, G or B from the array that stores the rgb values.
 * @param a is an integer that selects the color, its values can be between 0 and 2. 0 selects the red component, 1 selects the green component, 2 selects the blue component of the rgb.
 * @return a string containing the integer representation of the value of the requested color.
 */
std::string RGB::getColor(int a) const
{
    return kleuren[a];
}
