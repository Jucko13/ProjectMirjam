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
	system("killall eibd");
	system("su --login pi -c \"eibd -i ipt:10.0.2.2\" &");
}

/**
 * setvalues creates a string that are then thrown into the console
 * @brief KnxSensors::setValues sets the rgb values for the knx led.
 * @param r integer value for the red part, value can be between 0 and 255.
 * @param g integer value for the green part, value can be between 0 and 255.
 * @param b integer value for the blue part, value can be between 0 and 255.
 */
void KnxSensors::groupWrite(std::string group, std::string value){
	FILE *in;
        std::string command ="groupwrite ip:localhost " + group + " " + value;
	
	if(!(in = popen(command.c_str(), "r"))){
		return;
	}
	
	pclose(in);
}
