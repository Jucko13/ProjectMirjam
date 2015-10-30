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
	std::cout << "[ KNX    ] Kill Running EIBD...\r\n";
	system("killall -q eibd");
	
	std::cout << "[ KNX    ] Starting EIBD...\r\n";
	system("su --login pi -c \"eibd -i ipt:10.0.2.2\" &");
}

/**
 * groupWrite allows to select a knx group and send a value to it.
 * @brief KnxSensors::groupWrite
 * @param group selects the knxgroup
 * @param value sets the value of the chosen group
 */
void KnxSensors::groupWrite(std::string group, std::string value){
	FILE *in;
    std::string command ="groupwrite ip:localhost " + group + " " + value;
	
	if(!(in = popen(command.c_str(), "r"))){
		return;
	}
	
	pclose(in);
}
