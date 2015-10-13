#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;


		
class func {
	public:
	template <class T> static
	string toString(T item){
		ostringstream oss;
		oss << item;
		return (oss.str());
	}
	static string getIpAddress();
	static string openFile(const char * fileName);
	static string openBinaryFile(const char *fileName, int* fsize);
	static int getUrl (string str, string *url);
	static vector<string> &split(const string &s, char delim, vector<string> &elems);
	static int countChars(string input, char whatChar);

};


#endif
