#include "knxsensors.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "functions.h"

KnxSensors::KnxSensors()
{
	init();
}

KnxSensors::~KnxSensors()
{
    //dtor
}

/**
 * init initialises kills all eibd processes, then starts an eibd process as user pi
 * @brief KnxSensors::init is used to start eibd as user pi for knx
 */
void KnxSensors::init(){
	FILE *in;
	if(!(in = popen("killall eibd", "r"))){
		return;
	}
        if(!(in = popen("user pi", "r"))){
                return;
        }
        if(!(in = popen("eibd -i ipt:10.0.2.2", "r"))){
                return;
        }
	//login als pi en dan: 'eibd -i ipt:10.0.2.2' uitvoeren
	pclose(in);
}

/**
 * setvalues creates a string that are then thrown into the console
 * @brief KnxSensors::setValues sets the rgb values for the knx led.
 * @param r integer value for the red part, value can be between 0 and 255.
 * @param g integer value for the green part, value can be between 0 and 255.
 * @param b integer value for the blue part, value can be between 0 and 255.
 */
void KnxSensors::setValues(int r, int g, int b){
	FILE *in;
        std::string command ="groupwrite ip:localhost 1/2/4 " + func::toString(r) + " " + func::toString(g) + " " + func::toString(b);
	
	if(!(in = popen(command.c_str(), "r"))){
		return;
	}
	
	pclose(in);
}
