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
    vector<string> temp;
    ifstream myfile;
    if (myfile.is_open()){
        while(getline(myfile,line)){
            temp.push_back(line);
        }
        myfile.close();
    }
    for(int i = temp.size(); i > 0; --i){
        istringstream iss(temp[i]);
        string sub1;
        iss >> sub1;
        settingsInt.push_back(std::stoi(sub1));
        string sub2;
        iss >> sub2;
        settingsString.push_back(sub2);
    }
}

/**
 * @brief FileReader::writeFile
 * the writeFile function write the contents of the settings vector into the settings.txt file separated by an endline character.
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
 * @param i: the integer that determines what wekker is selected
 * @param string s: is used to to edit the value of the time settings
 */
void FileReader::changeSettings(int i, string s){
    settingsInt.push_back(i);
    settingsString.push_back(s);
}

/**
 * @brief FileReader::getTime
 * the getTime functions allows to retreive the saved time setting for a particular setting slot
 * @param integer a: is used to select which time setting is to be requested
 * @return the time is returned as an integer, if the time was not set, 1 is returned.
 */
void FileReader::getTime(unsigned int i, int *w, string *s){
    if(settingsInt.size()<=i && settingsString.size()<=i){
        *w = settingsInt[i];
        *s = settingsString[i];
    }
    else{
        *w=0;
        *s="";
    }
}

/**
 * @brief FileReader::clearSettings clears all settings currently in the vectors.
 */
void FileReader::clearSettings(){
    while(settingsInt.size()){
        settingsInt.pop_back();
    }
    while(settingsString.size()){
        settingsString.pop_back();
    }
}
