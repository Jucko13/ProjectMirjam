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
 * @brief RGB::setColorString requires 4 strings representing the values for red green, blue and white in the rgb spectrum. It will then call KnxSensors::groupWrite() to write this color to the KNX.
 * @param r is a string representation of the value for the red component of rgb. This value can be between 0 and 99.
 * @param g is a string representation of the value for the green component of rgb. This value can be between 0 and 99.
 * @param b is a string representation of the value for the blue component of rgb. This value can be between 0 and 99.
 * @param w is a string representation of the value for the white component of rgb. This value can be between 0 and 99.
 */
void RGB::setColorString(std::string r, std::string g, std::string b, std::string w)
{
	kleuren[0] = r;
	kleuren[1] = g;
	kleuren[2] = b;
	kleuren[3] = w;
	
	knx->groupWrite("1/2/4", r + " " + g + " " + b);
	knx->groupWrite("1/2/3", w);

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
