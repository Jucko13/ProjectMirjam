#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

FileReader::FileReader() {
    readFile();
}

FileReader::~FileReader() {

}

/**
 * @brief FileReader::readFile
 * the readFile function reads the settings from the settings.txt file into the vector settings.
 */
void FileReader::readFile(){
    string line;
    ifstream myfile;
    if (myfile.is_open()){
        while(getline(myfile,line)){
            settings.push_back(line);
        }
        myfile.close();
    }
    /*
    else {
        for(int i = 0; i < 10; i++){
            settings.push_back(".");
        }
    }
    */
}

/**
 * @brief FileReader::writeFile
 * the writeFile function write the contents of the settings vector into the settings.txt file separated by an endline character.
 */
void FileReader::writeFile(){
    ofstream myfile;
    myfile.open("settings.txt");
    for(unsigned int i = 0; i < settings.size(); i++){
        myfile<<settings[i];
        myfile<<"\n";
    }
    myfile.close();
}

/**
 * @brief FileReader::changeSettings
 * the changeSettings function allows to change a timeslot in the settings.
 * @param unsinged integer i: is used to selects which setting is changed
 * @param string s: is used to to edit the value of the time settings
 */
void FileReader::changeSettings(unsigned int i, string t){
    if(settings.size()<i){
		for(unsigned int a = settings.size(); a < i; a++){
			settings.push_back(".");
		}
	}
    settings[i] = t;
}

/**
 * @brief FileReader::getTime
 * the getTime functions allows to retreive the saved time setting for a particular setting slot
 * @param integer a: is used to select which time setting is to be requested
 * @return the time is returned as an integer, if the time was not set, 1 is returned.
 */
string FileReader::getTime(unsigned int i){
    string time;
    if(settings.size()<i){
		return ".";
	}
    time = settings[i];
    return time;
}
