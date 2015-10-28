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

//if we ever going to do something with the pictures of the camera: this is how you get all files in a folder for the html slideshow: "dir /b"

//defines for checkUrlMode();
#define TTS_TIME 0
#define TTS_TOGGLE 1
#define TTS_SET 2

#include "src/IvySox.h"
#include "src/arduPi.h"
#include "src/functions.h"
#include "src/door.h"
#include "src/light.h"
#include "src/wekker.h"
#include "src/sunblind.h"
#include "src/motionsensor.h"
#include "src/buzzer.h"
#include "src/FileReader.h"
#include "src/knxsensors.h"
#include "src/rgb.h"

#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>

vector<MotionSensor *> * motionPointer = NULL;
vector<Buzzer *> * buzzerPointer = NULL;

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

enum devtype { LAMPS, SUNBLINDS };

/**
 * @brief modifyWekkers
 * @param wekkers
 * @param modifyWekkerData
 * @param time
 */
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

/**
 * @brief saveWekkers
 * @param wekkers
 * @param fr
 */
void saveWekkers(vector<Wekker*>& wekkers, FileReader &fr) {
	fr.clearSettings();
	
	for (int t = 0; t < wekkers.size(); t++) {
		fr.changeSettings(wekkers[t]->getData(), wekkers[t]->getTime());
	}

	fr.writeFile();
}

/**
 * @brief restoreWekkers
 * @param wekkers
 * @param fr
 */
void restoreWekkers(vector<Wekker*>& wekkers, FileReader &fr) {
	int data = 0;
	std::string time;
	int t = 0;

	//fr.readFile();
	fr.getTimeSetting(t, &data, &time);

	while(time != ""){
		wekkers.push_back(new Wekker(time.c_str(), true, data));
		fr.getTimeSetting(++t, &data, &time);
	}
}

/**
 * @brief checkAllWekkers
 * @param wekkers
 * @param lamp
 * @param sunblinds
 */
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


// 0; /toggle/
// 1; /set/
// 2; /time/
/**
 * @brief checkUrlMode
 * @param pageurl
 * @param sensorNumber
 * @param sensorData
 * @param sensorState
 * @return
 */
int checkUrlMode(std::string pageurl, int * sensorNumber, string * sensorData, int * sensorState){
	std::string tmpString;
	std::size_t tmpPlace;
	
	int returnmode = -1;
	returnmode = (pageurl.find("/toggle/") != std::string::npos ? TTS_TOGGLE : returnmode);
	returnmode = (returnmode == -1 && pageurl.find("/set/") != std::string::npos ? TTS_SET : returnmode);
	returnmode = (returnmode == -1 && pageurl.find("/time/") != std::string::npos ? TTS_TIME : returnmode);
	
	switch(returnmode){
		case TTS_TOGGLE:
			if (func::countChars(pageurl, '/') != 3) return -1;
			tmpPlace = pageurl.rfind("/");
			tmpString = pageurl.substr(tmpPlace + 1, pageurl.length() - tmpPlace); if (tmpString.size() == 0) return -1;
			*sensorNumber = atoi(tmpString.c_str());
			return returnmode;
			break;

		case TTS_SET:
			if (func::countChars(pageurl, '/') != 4) return -1;
			tmpPlace = pageurl.rfind("/");
			*sensorData = pageurl.substr(tmpPlace + 1, pageurl.length() - tmpPlace);

			tmpPlace = pageurl.rfind("/", tmpPlace - 1);
			tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
			*sensorNumber = atoi(tmpString.c_str());
			return returnmode;
			break;

		case TTS_TIME:
			if (func::countChars(pageurl, '/') != 5) return -1;
			tmpPlace = pageurl.rfind("/");
			*sensorData = pageurl.substr(tmpPlace + 1, pageurl.length() - tmpPlace);
			tmpPlace = pageurl.rfind("/", tmpPlace - 1);
			tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
			*sensorState = atoi(tmpString.c_str());
			tmpPlace = pageurl.rfind("/", tmpPlace - 1);
			tmpString = pageurl.substr(tmpPlace + 1, 1); if (tmpString.size() == 0) return -1;
			*sensorNumber = atoi(tmpString.c_str());
			return returnmode;
			break;

		default:
			return -1;
			break;
	}

	return -1;
}


/**
 * @brief triggerMotion
 */
void triggerMotion(){
	if (!motionPointer) return;
	for(int i = 0; i < motionPointer->size(); i++){
		(*motionPointer)[i]->setStatus();
	}
}


/**
 * @brief triggerButton
 */
void triggerButton(){
	if (!buzzerPointer) return;
	for(int i = 0; i < buzzerPointer->size(); i++){
		(*buzzerPointer)[i]->playAlarm();
	}
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
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
	vector<MotionSensor *> motion;
	motionPointer = &motion;
	vector<Sunblind *> sunblinds;
	vector<Buzzer *> buzzers;
	buzzerPointer = &buzzers;

	KnxSensors k;
	RGB ledStrip(&k);
	FileReader fr;
	
	vector<Wekker *> wekkers;
	wekkercompressed wekkerData;
	
	restoreWekkers(wekkers, fr);
	
	attachInterrupt(5, triggerButton, RISING);
	attachInterrupt(6, triggerButton, RISING);

	motion.push_back(new MotionSensor(2, triggerMotion));
	motion.push_back(new MotionSensor(3, triggerMotion));
	motion.push_back(new MotionSensor(4, triggerMotion));
	
	sunblinds.push_back(new Sunblind(12));
	
	door.push_back(new Door(8));
	
	lamp.push_back(new Light(9));
	lamp.push_back(new Light(10));
	lamp.push_back(new Light(11));
	
	buzzers.push_back(new Buzzer(13));	
	std::cout << "[ SERVER ] Creating Server...\r\n";
    if (listener.openServerOnPort(PORTNUMBER) < 0 || listener.listenToPort() < 0){
        std::cout << "[ ERROR  ] Error Starting server, port " << PORTNUMBER << " is in use!\r\n";
        exit(-1);
    }
	
    std::cout << "[ SERVER ] Server Created! URL: http://" << func::getIpAddress() << ":" << func::toString(PORTNUMBER) << "\r\n";
	std::cout << "[ SOCKET ] Waiting for incomming connections...\r\n";
	
	while(1){
		checkAllWekkers(wekkers, lamp, sunblinds);	
		
		
		
		/*##########################################################*/
		usleep(50000);
		for (int t = 0; t < motion.size(); t++) { motion[t]->timeCheck(); }
		
        int socketNumber = listener.acceptInbound(inbound);
        if(socketNumber == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) continue;
		
		connectionCounter++;
        std::cout << "[ SOCKET ] Received request from " << inbound->getIpAddress() << "\r\n";
        
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
			response += "\r\n\r\n";
			
			inbound->sendMessage(response);
			inbound->closeConnection();
			continue;
		}else if(pageurl.find(".png") != std::string::npos){
			response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: image/png;\r\n\r\n";
			
			inbound->sendFile("html" + pageurl);
			inbound->closeConnection();
			continue;
		}else if(pageurl.find(".jpg") != std::string::npos){
			response = "HTTP/1.1 200 OK\r\n";
			response += "Content-Type: image/jpg;\r\n\r\n";
			
			inbound->sendFile("html/camera" + pageurl);
			inbound->closeConnection();
			continue;
		}
		
		response = "HTTP/1.1 200 OK\r\n\r\n";
		
		if(pageurl.find("/all/") != std::string::npos){
			for (int t = 0; t < door.size(); t++){ response += "data.door[" + func::toString(t) + "] = " + func::toString(door[t]->getStatus()) + ";\r\n"; }
			for (int t = 0; t < lamp.size(); t++) { response += "data.lamp[" + func::toString(t) + "] = " + func::toString(lamp[t]->getStatus()) + ";\r\n"; }
			for (int t = 0; t < motion.size(); t++) { response += "data.motion[" + func::toString(t) + "] = " + func::toString(motion[t]->getStatus()) + ";\r\n"; }
			for (int t = 0; t < sunblinds.size(); t++) { response += "data.sunblinds[" + func::toString(t) + "] = " + func::toString(sunblinds[t]->getPosition()) + ";\r\n"; }
			for (int t = 0; t < 4; t++) { response += "data.rgb[" + func::toString(t) + "] = " + ledStrip.getColor(t) + ";\r\n"; }

		}else if (pageurl.find("/door/") != std::string::npos) {
			switch (checkUrlMode(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case TTS_TOGGLE:
					if(door[objectIndex]->getStatus()) {
						door[objectIndex]->close();
					}else{
						door[objectIndex]->open();
					}
					response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(door[objectIndex]->getStatus()) + ";"; 
					break;

				case TTS_SET:
					if(objectSettings == "1") {
						door[objectIndex]->open();
					}else{
						door[objectIndex]->close();
					}
					response += "data.door[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
			}
		}else if (pageurl.find("/rgb/") != std::string::npos) {
			switch (checkUrlMode(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case TTS_SET:
					if (func::countChars(objectSettings,'_') == 3){
						std::vector<std::string> rgbColors;
						func::split(objectSettings, '_', rgbColors);

						ledStrip.setColorString(rgbColors[0], rgbColors[1], rgbColors[2], rgbColors[3]);
					}
					break;
			}
		}else if(pageurl.find("/lamp/") != std::string::npos){
			switch (checkUrlMode(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case TTS_TOGGLE:
					lamp[objectIndex]->toggle();
					response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;

				case TTS_SET:
					if(objectSettings == "1") {
						lamp[objectIndex]->on();
					}else{
						lamp[objectIndex]->off();
					}
					response += "data.lamp[" + func::toString(objectIndex) + "] = " + func::toString(lamp[objectIndex]->getStatus()) + ";"; 
					break;
				
				case TTS_TIME:
					wekkerData.uncomp.devicetype = LAMPS;
					wekkerData.uncomp.deviceindex = objectIndex;
					wekkerData.uncomp.state = objectState;
					modifyWekkers(wekkers, wekkerData, objectSettings);
					saveWekkers(wekkers, fr);
					response += "timeSaved();";
					response += "setTimeout(function(){addTimedCommand('time/times/0/0/0');}, 10);\r\n";
					break;

			}
		}else if(pageurl.find("/sunblinds/") != std::string::npos){
			switch (checkUrlMode(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case TTS_TOGGLE:
					sunblinds[objectIndex]->toggle();
					response += "data.sunblinds[" + func::toString(objectIndex) + "] = " + func::toString(sunblinds[objectIndex]->getStatus()) + ";"; 
					break;

				case TTS_SET:
					sunblinds[objectIndex]->setPosition(atoi(objectSettings.c_str()));
					response += "data.sunblinds[" + func::toString(objectIndex) + "] = " + func::toString(sunblinds[objectIndex]->getPosition()) + ";"; 
					break;
				
				case TTS_TIME:
					wekkerData.uncomp.devicetype = SUNBLINDS;
					wekkerData.uncomp.deviceindex = objectIndex;
					wekkerData.uncomp.state = objectState;
					modifyWekkers(wekkers, wekkerData, objectSettings);
					saveWekkers(wekkers, fr);
					response += "timeSaved();";
					response += "setTimeout(function(){addTimedCommand('time/times/0/0/0');}, 10);\r\n";
					break;
			}
		}else if(pageurl.find("helpme/") != std::string::npos){
			//todo: implement buzzer
			for (int t = 0; t < buzzers.size(); t++) {
				buzzers[t]->playAlarm();
			}

		} else if (pageurl.find("/times/") != std::string::npos) {
			switch (checkUrlMode(pageurl, &objectIndex, &objectSettings, &objectState)) {
				case TTS_TIME:
					for (int t = 0; t < wekkers.size(); t++) {
						wekkerData.comp = wekkers[t]->getData();

						switch (wekkerData.uncomp.devicetype) {
							case LAMPS:
								response += "times.lamp[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][" + func::toString((int)wekkerData.uncomp.state) + "] = \"" + wekkers[t]->getTime() + "\";\r\n";
								response += "times.lamp[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][2] = 1;\r\n";
								break;
							case SUNBLINDS:
								response += "times.sunblinds[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][" + func::toString((int)wekkerData.uncomp.state) + "] = \"" + wekkers[t]->getTime() + "\";\r\n";
								response += "times.sunblinds[" + func::toString((int)wekkerData.uncomp.deviceindex) + "][2] = 1;\r\n";
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
        std::cout << "[ SOCKET ] Connection " << connectionCounter << " closed. Waiting...\r\n";
        std::cout.flush();
	};
	
    exit(0);
}


