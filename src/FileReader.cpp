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
 * the readFile function reads the settings from the settings.txt file into two vectors.
 */
void FileReader::readFile(){
    string line;
    vector<string> temp;
    ifstream myfile("settings.txt");
	clearSettings();
	
    if (myfile.is_open()){
        while(getline(myfile,line)){
            temp.push_back(line);
        }
        myfile.close();
    }

    
    for(int i = 0; i < temp.size(); i++){
        istringstream iss(temp[i]);
        string sub1;
        iss >> sub1;
        settingsInt.push_back(atoi(sub1.c_str()));
        string sub2;
        iss >> sub2;
        settingsString.push_back(sub2);
    }
}

/**
 * @brief FileReader::writeFile
 * the writeFile function write the contents of the settings vectors into the settings.txt file separated by an endline character.
 */
void FileReader::writeFile(){
    ofstream myfile;
    string temp;
    myfile.open("settings.txt");
    for(unsigned int i = 0; i < settingsInt.size(); i++){
        ostringstream oss;
        oss << settingsInt[i];
        temp = oss.str();
        myfile << temp << " " << settingsString[i] << endl;
    }
    myfile.close();
}

/**
 * @brief FileReader::changeSettings
 * the changeSettings function allows to change a timeslot in the settings.
 * @param i: the integer idenentifier that determines which wekker is selected.
 * @param string s: is used to to edit the value of the time settings.
 */
void FileReader::changeSettings(int i, string s){
    settingsInt.push_back(i);
    settingsString.push_back(s);
}

/**
 * @brief FileReader::getTime
 * the getTime functions allows to retreive the saved time setting for a particular setting slot.
 * @param unsinged integer i: is used to select which setting is to be retreived.
 * @param integer *i: is used to retreive the integer value that determines which wekker is selected by the setting.
 * @param string *s: is used to retreive the time that corresponds to the requested setting.
 */
void FileReader::getTimeSetting(unsigned int i, int *w, string *s){
    if(i < settingsInt.size() && i< settingsString.size()){
        *w = settingsInt[i];
        *s = settingsString[i];
    }else{
        *w=0;
        *s="";
    }
}

/**
 * @brief FileReader::clearSettings clears all settings currently in the vectors.
 */
void FileReader::clearSettings(){
	settingsInt.clear();
	settingsString.clear();
}
