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
#include "src/sunblind.h"
#include "src/motionsensor.h"
//#include "src/knxsensors.h"

#include <unistd.h>
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

enum devtype { LAMPS, SUNBLINDS, TOILETALARM };

void modifyWekkers(vector<Wekker*>& wekkers, wekkercompressed modifyWekkerData, string time){
	wekkercompressed wekkerData;
	int t = 0;
	modifyWekkerData.uncomp.reserved = 0;
	for (; t < wekkers.size(); t++) {
		wekkerData.comp = wekkers[t]->getData();
		wekkerData.uncomp.reserved = 0;

		if (wekkerData.comp == modifyWekkerData.comp) {
			if (time == "-1") {//delete wekker
				delete wekkers[t];
				wekkers.erase(wekkers.begin() + t);
			} else {//change wekker time
				wekkers[t]->setTime(time.c_str());
			}
			break;//end the for loop
		}
	}
	if (t >= wekkers.size()) {// create new wekker
		wekkers.push_back(new Wekker(time.c_str(), true, modifyWekkerData.comp));//absolute repeated
	}
}

void checkAllWekkers(vector<Wekker*>& wekkers, vector<Light *> &lamp, vector<Sunblind *> &sunblinds){
	wekkercompressed wekkerData;
	for (vector<Wekker *>::iterator i = wekkers.begin(); i != wekkers.end(); /*i++*/) {
		if ((*i)->isDue()) {
			wekkerData.comp = (*i)->getData();
			switch(wekkerData.uncomp.devicetype){
				case LAMPS:
					if(wekkerData.uncomp.state != 0) {
						lamp[wekkerData.uncomp.deviceindex]->on();
					}else{
						lamp[wekkerData.uncomp.deviceindex]->off();
					}
					break;
				case SUNBLINDS:
					if(wekkerData.uncomp.state != 0) {
						sunblinds[wekkerData.uncomp.deviceindex]->open();
					}else{
						sunblinds[wekkerData.uncomp.deviceindex]->close();
					}
					break;
				case TOILETALARM:
					break;
			}
			if ((*i)->isRepeating()) {//recuring
				(*i)->recalculateTime();
				i++;
			} else {//runonce
				delete *i;
				i = wekkers.erase(i);
			}
		} else {
			i++;
		}
	}
}


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
		tmpPlace = pageurl.rfind("/",tmpPlace-1);
		tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
		*sensorState = atoi(tmpString.c_str());
		tmpPlace = pageurl.rfind("/",tmpPlace-1);
		tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
		*sensorNumber = atoi(tmpString.c_str());
		return returnmode;
	}
	return -1;
}



vector<MotionSensor *> * blatest = NULL;

void triggerMotion(){
	if (blatest != NULL) {
		(*blatest)[0]->setStatus();
	}
}

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
	vector<MotionSensor *> motion;
	blatest = &motion;
	vector<Sunblind *> sunblinds;

	vector<Wekker *> wekkers;
	wekkercompressed wekkerData;
	
	//KnxSensors k;
	//k.test(argc, argv);
	


	motion.push_back(new MotionSensor(2, triggerMotion));
	
	sunblinds.push_back(new Sunblind(6));
	
	door.push_back(new Door(7));
	
	lamp.push_back(new Light(11));
	lamp.push_back(new Light(10));
	lamp.push_back(new Light(9));
	
	//wekkerData.uncomp.devicetype = LAMPS;
	//wekkerData.uncomp.deviceindex = 1;
	//wekkerData.uncomp.state = 1;
	//wekkerData.uncomp.reserved = 0;
	//wekkers.push_back(new Wekker(1, wekkerData.comp));//relative 1 min from now

	std::cout << "Creating Server...\r\n";
    if (listener.openServerOnPort(PORTNUMBER) < 0 || listener.listenToPort() < 0){
        std::cout << "Error opening port " << PORTNUMBER << "!\r\n";
        exit(-1);
    }
	
    std::cout << "Server Created! URL: http://" << func::getIpAddress() << ":" << func::toString(PORTNUMBER) << "\r\n" << "Waiting for incomming connections...\r\n";

	while(1){
		checkAllWekkers(wekkers, lamp, sunblinds);	
		
		
		
		/*##########################################################*/
		usleep(100000);
		for (int t = 0; t < motion.size(); t++) { motion[t]->timeCheck(); }
		
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
			
			inbound->sendFile("html" + pageurl);
			inbound->closeConnection();
			continue;
		}
		
		response = "HTTP/1.1 200 OK\r\n\r\n";
		
		if(pageurl.find("/all/") != std::string::npos){
			for (int t = 0; t < door.size(); t++){ response += "data.door[" + func::toString(t) + "] = " + func::toString(door[t]->getStatus()) + ";\r\n"; }
			for (int t = 0; t < lamp.size(); t++) { response += "data.lamp[" + func::toString(t) + "] = " + func::toString(lamp[t]->getStatus()) + ";\r\n"; }
			for (int t = 0; t < motion.size(); t++) { response += "data.motion[" + func::toString(t) + "] = " + func::toString(motion[t]->getStatus()) + ";\r\n"; }
		
		}else if (pageurl.find("/door/") != std::string::npos) {
			switch(get_toggle_set(pageurl,&objectIndex,&objectSettings,&objectState)){
				//case 0:
					//response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(door[objectIndex]->getStatus()) + ";"; 
					//break;
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
				//case 0:
					//response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					//break;
				case 1:
					//cout<<objectIndex;
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
					//objectSettings="00:00";
					//objectState = 0; //0 moet aan gaan om <tijd>, 1 moet uit gaan om <tijd>
					//objectIndex = index van lamp
					wekkerData.uncomp.devicetype = LAMPS;
					wekkerData.uncomp.deviceindex = objectIndex;
					wekkerData.uncomp.state = objectState;
					modifyWekkers(wekkers, wekkerData, objectSettings);
					
					response += "timeSaved();";
					break;
			}
		}else if(pageurl.find("/sunblinds/") != std::string::npos){
			switch(get_toggle_set(pageurl,&objectIndex,&objectSettings,&objectState)){
				//case 0:
					//response += "data.blinds[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					//break;
				case 1:
					
					sunblinds[objectIndex]->toggle();
					response += "data.sunblinds[" + func::toString(objectIndex) + "] = " + func::toString(sunblinds[objectIndex]->getStatus()) + ";"; 
					break;
				case 2:
					if(objectSettings == "1") {
						sunblinds[objectIndex]->open();
					}else{
						sunblinds[objectIndex]->close();
					}
					response += "data.sunblinds[" + func::toString(objectIndex) + "] = " + func::toString(sunblinds[objectIndex]->getStatus()) + ";"; 
					break;
				
				case 3:
					wekkerData.uncomp.devicetype = SUNBLINDS;
					wekkerData.uncomp.deviceindex = objectIndex;
					wekkerData.uncomp.state = objectState;
					modifyWekkers(wekkers, wekkerData, objectSettings);
					
					response += "timeSaved();";
					break;
			}
		} else if (pageurl.find("/times/") != std::string::npos) {
			switch (get_toggle_set(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case 3:
					for (int t = 0; t < wekkers.size(); t++) {
						wekkerData.comp = wekkers[t]->getData();

						switch (wekkerData.uncomp.devicetype) {
							case LAMPS:
								response += "times.lamp[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][" + func::toString((int)wekkerData.uncomp.state) + "] = \"" + wekkers[t]->getTime() + "\";\r\n";
								response += "times.lamp[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][2] = 1;\r\n";
								break;
							case TOILETALARM:
								break;
						}
					}

					response += "timeSelectionChange();\r\n";

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


