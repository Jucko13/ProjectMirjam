/*

[OliWeb and IvySox are provided under the MIT software license]

Copyright (C) 2013 Jeffrey Moore        

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/
#define ERR_MESSAGE "message.show('unknown request!'); setTimeout(function(){ message.hide();},1500);"

#include "src/IvySox.h"
#include "src/arduPi.h"
#include "src/functions.h"
#include "src/door.h"
#include "src/light.h"
#include "src/wekker.h"
//#include "src/knxsensors.h"

#include <iostream>
#include <string>
#include <cstring>
//#include <pthread.h>

#include <vector>


#define PORTNUMBER 8077
#define INBOUND_BUFFER_SIZE 20000

typedef union {
	struct {
		unsigned char devicetype;
		unsigned char deviceindex;
		unsigned char state;
		unsigned char reserved;
	} uncomp;
	int comp;
} wekkercompressed;

enum devtype { LAMPS, TOILETALARM };


int countChars(string input, char whatChar){
	int howmany = 0;
	for(int i = 0; i < input.size(); i++){
		if(input[i] == whatChar){
			howmany++;
		}
	}
	return howmany;
}

// 0; /get/
// 1; /toggle/
// 2; /set/
// 3; /time/
int get_toggle_set(std::string pageurl, int * sensorNumber, string * sensorData, int * sensorState){
	std::string tmpString;
	std::size_t tmpPlace;
	
	int returnmode = -1;
	returnmode = (pageurl.find("/get/") != std::string::npos ? 0 : returnmode);
	returnmode = (returnmode == -1 && pageurl.find("/toggle/") != std::string::npos ? 1 : returnmode);
	returnmode = (returnmode == -1 && pageurl.find("/set/") != std::string::npos ? 2 : returnmode);
	returnmode = (returnmode == -1 && pageurl.find("/time/") != std::string::npos ? 3 : returnmode);
	
	if(returnmode == 0 || returnmode == 1){
		if(countChars(pageurl,'/') != 3) return -1;
		tmpPlace = pageurl.rfind("/");
		tmpString = pageurl.substr(tmpPlace + 1, pageurl.length() - tmpPlace); if (tmpString.size() == 0) return -1;
		*sensorNumber = atoi(tmpString.c_str());
		return returnmode;
	}
	
	if(returnmode == 2){
		if(countChars(pageurl,'/') != 4) return -1;
		tmpPlace = pageurl.rfind("/");
		*sensorData = pageurl.substr(tmpPlace+1,pageurl.length() - tmpPlace);

		tmpPlace = pageurl.rfind("/",tmpPlace);
		tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
		*sensorData = atoi(tmpString.c_str());
		return returnmode;
	}
	
	if(returnmode == 3){
		if(countChars(pageurl,'/') != 5) return -1;
		tmpPlace = pageurl.rfind("/");
		*sensorData = pageurl.substr(tmpPlace+1,pageurl.length() - tmpPlace);
		tmpPlace = pageurl.rfind("/",tmpPlace);
		tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
		*sensorState = atoi(tmpString.c_str());
		tmpPlace = pageurl.rfind("/",tmpPlace);
		tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
		*sensorNumber = atoi(tmpString.c_str());
		return returnmode;
	}
	return -1;
}

/*	
void * connectionThread(void * dummyptr){
	
    while (1){
		
        
        
    }
	
}
*/


int main (int argc, char *argv[])
{
	InboundConnection inboundConnection;
    InboundConnection *inbound = &inboundConnection;
    IvySox listener;
	
	string response;
    string pageurl;
    string objectSettings;
    int objectIndex = 0;
    int objectState = 0;
    char messageBuffer[INBOUND_BUFFER_SIZE];
	
	int connectionCounter = 0;
	
	vector<Door *> door;
	vector<Light *> lamp;
	
	vector<Wekker *> wekkers;
	wekkercompressed wekkerdata;
	
	//KnxSensors k;
	//k.test();
	
	/*pthread_t webhost;
	
	std::cout << "Creating Connection Thread...\r\n";
	if(pthread_create (&webhost, NULL, connectionThread, NULL) != 0){
		
		std::cout << "Thread creation failed. Shutting down...\r\n";
		exit(0);
	}
	std::cout << "Thread creation successfull!\r\n";
	*/
	
	

	door.push_back(new Door(7));
	door.push_back(new Door(6));
	door.push_back(new Door(5));
	
	lamp.push_back(new Light(11));
	lamp.push_back(new Light(10));
	lamp.push_back(new Light(9));
	
	wekkerdata.uncomp.devicetype = LAMPS;
	wekkerdata.uncomp.deviceindex = 1;
	wekkerdata.uncomp.state = 1;
	wekkerdata.uncomp.reserved = 0;
	wekkers.push_back(new Wekker(1, wekkerdata.comp));//relative 1 min from now

	wekkerdata.uncomp.devicetype = LAMPS;
	wekkerdata.uncomp.deviceindex = 1;
	wekkerdata.uncomp.state = 0;
	wekkerdata.uncomp.reserved = 0;
	wekkers.push_back(new Wekker("13:10", true, wekkerdata.comp));//absolute repeated
	
	std::cout << "Creating Server...\r\n";
    if (listener.openServerOnPort(PORTNUMBER) < 0 || listener.listenToPort() < 0){
        std::cout << "Error opening port " << PORTNUMBER << "!\r\n";
        exit(-1);
    }
	
    std::cout << "Server Created! URL: http://" << func::getIpAddress() << ":" << func::toString(PORTNUMBER) << "\r\n" << "Waiting for incomming connections...\r\n";

	while(1){
		
		for (vector<Wekker *>::iterator i = wekkers.begin(); i != wekkers.end(); /*i++*/) {
			if ((*i)->isdue()) {
				//cout << ((*i)->gettime()) << endl;
				wekkerdata.comp = (*i)->getdata();
				switch(wekkerdata.uncomp.devicetype){
					case LAMPS:
						if(wekkerdata.uncomp.state != 0) {
							lamp[wekkerdata.uncomp.deviceindex]->on();
						}else{
							lamp[wekkerdata.uncomp.deviceindex]->off();
						}
						break;
					case TOILETALARM:
						break;
				}
				if ((*i)->isrepeating()) {//runonce or recuring
					(*i)->recalculatetime();
					i++;
				} else {
					delete *i;
					i = wekkers.erase(i);
				}
			} else {
				i++;
			}
		}
		
		
		
		
		/*##########################################################*/
		delay(100);
        int socketNumber = listener.acceptInbound(inbound);
        if(socketNumber == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) continue;
		
		connectionCounter++;
        std::cout << "Received request from " << inbound->getIpAddress() << "\r\n";
        
        size_t bytesReceived = inbound->receive(messageBuffer, INBOUND_BUFFER_SIZE);
        string message = listener.messageToString(messageBuffer, bytesReceived);
        
		if(!func::getUrl(message,&pageurl)){ //unknown page url
			response = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
			inbound->sendMessage(response);
			inbound->closeConnection();
			continue;
		}
		
		
		response = "HTTP/1.1 200 OK\r\n\r\n";
		//response += "pageurl: " + pageurl + "</br>\r\n";
		
		if(pageurl.compare("/") == 0 || pageurl.compare("/index.htm") == 0){
			response = "HTTP/1.1 200 OK\r\n\r\n";
			response += func::openFile("html/top.htm");
			response += func::openFile("html/index.htm");
			response += func::openFile("html/bottom.htm");
			
			inbound->sendMessage(response);
			inbound->closeConnection();
			continue;
		}else if(pageurl.find(".png") != std::string::npos){
			response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: image/png;\r\n\r\n";
			/*
			int fsize = 0;
			string filesend = func::openBinaryFile("html/boost_build.png",&fsize;
			inbound->sendMessage(response);
			inbound->sendMessage(&filesend, fsize);
			* */
			
			inbound->sendFile("html" + pageurl);
			inbound->closeConnection();
			continue;
		}
		
		response = "HTTP/1.1 200 OK\r\n\r\n";
		if(pageurl.find("/door/") != std::string::npos){
			switch(get_toggle_set(pageurl,&objectIndex,&objectSettings,&objectState)){
				case 0:
					response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(door[objectIndex]->getStatus()) + ";"; 
					break;
				case 1:
					if(door[objectIndex]->getStatus()) {
						door[objectIndex]->close();
					}else{
						door[objectIndex]->open();
					}
					response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(door[objectIndex]->getStatus()) + ";"; 
					break;
				case 2:
					if(objectSettings == "1") {
						door[objectIndex]->open();
					}else{
						door[objectIndex]->close();
					}
					response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
			}
		}else if(pageurl.find("/lamp/") != std::string::npos){
			switch(get_toggle_set(pageurl,&objectIndex,&objectSettings,&objectState)){
				case 0:
					response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
				case 1:
					
					lamp[objectIndex]->toggle();
					response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
				case 2:
					if(objectSettings == "1") {
						lamp[objectIndex]->on();
					}else{
						lamp[objectIndex]->off();
					}
					response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
				
				case 3:
					//objectSettings contains your time. if its empty, disable the time
					break;
			}
		} else if (pageurl.find("/times/") != std::string::npos) {
			switch (get_toggle_set(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case 3:
					if (objectSettings == "") { //send all times back
						for (int t = 0; t != wekkers.size(); t++) {
							wekkerdata.comp = wekkers[t]->getdata();

							switch (wekkerdata.uncomp.devicetype) {
								case LAMPS:
									response += "timers.lamp[" + func::toString((int)wekkerdata.uncomp.deviceindex) + "][" + func::toString((int)wekkerdata.uncomp.state) + "] = \"" + wekkers[t]->gettime() + "\";\r\n";
									response += "timers.lamp[" + func::toString((int)wekkerdata.uncomp.deviceindex) + "][2] = 1;\r\n";
									break;
								case TOILETALARM:
									break;
							}
						}

						response += "timeSelectionChange();\r\n";
					} else { //set an timer

					}

					break;
			}
		}else{
			response += ERR_MESSAGE;
		}
		
        response += "\r\n\r\n";
        inbound->sendMessage(response);

        // Close the connection
        inbound->closeConnection();
        std::cout << "[" << connectionCounter << "]Waiting for incomming connections...\r\n";
        std::cout.flush();
	};
	
    exit(0);
}


