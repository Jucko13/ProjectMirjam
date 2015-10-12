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

void KnxSensors::init(){
	FILE *in;
	if(!(in = popen("killall eibd", "r"))){
		return;
	}
	//login als pi en dan: 'eibd -i ipt:10.0.2.2' uitvoeren
	pclose(in);
}

void KnxSensors::setValues(int a, int b, int c){
	FILE *in;
	std::string command ="groupwrite ip:localhost 1/2/4 " + func::toString(a) + " " + func::toString(b) + " " + func::toString(c);
	
	if(!(in = popen(command.c_str(), "r"))){
		return;
	}
	
	pclose(in);
}
