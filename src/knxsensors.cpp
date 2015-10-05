#include "knxsensors.h"
#include <knx/knx.hpp>

void KnxSensors::test()
{
	int argc = 1;
	char *argv[] = {"192.168.0.112"};
	
    knx::config config(argc, argv);
	knx::connection connection(config);
	knx::handle handle(connection);
	connection.set<knx::dpt_switch>(knx::group("1/2/3"), knx::dpt_switch::ON);
    
}

KnxSensors::KnxSensors()
{
	
}

KnxSensors::~KnxSensors()
{
    //dtor
}
