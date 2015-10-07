#include "knxsensors.h"
#include <knx/knx.hpp>

void KnxSensors::test(int argc, char *argv[])
{
	
    knx::config config(argc, argv);
    config.local_control_host = "10.0.1.3";
    config.local_control_port = 3617;
    config.local_data_host = "10.0.1.3";
    config.local_data_port = 3617;
    config.remote_host = "10.0.1.2";
    config.remote_port = 3617;
	config.logging_filename = "test.txt";
    config.logging_activated = false;
    
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
