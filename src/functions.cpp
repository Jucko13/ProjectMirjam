#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>

#include <cstring>
#include <string>
#include <vector>
using namespace std;

/*
template <class T>
string toString(T item){
	ostringstream oss;
	oss << item;
	return (oss.str());
}*/

std::vector<std::string> &func::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

int func::countChars(string input, char whatChar) {
	int howmany = 0;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == whatChar) {
			howmany++;
		}
	}
	return howmany;
}

int func::getUrl (string str, string *url)
{
  //string str = "GET /index.htm HTTP/1.1";

  size_t pos = str.find("GET");
  size_t pos2 = str.find("HTTP");


  if(pos != string::npos)
  {
      pos += 4;
  } else {
      return 0;
  }

  if(pos2 != string::npos)
  {
      pos2 -= 5;
  } else {
      return 0;
  }

  *url = str.substr(pos, pos2);

  if(url->size() != 0)
    return 1;
  else
    return 0;
}

string func::getIpAddress(){
	FILE * fp = popen("ifconfig eth0", "r");
	if (fp) {
		char *p=NULL, *e; size_t n;
		while ((getline(&p, &n, fp) > 0) && p) {
			if (p = strstr(p, "inet ")) {
				p+=5;
				if (p = strchr(p, ':')) {
					++p;
					if (e = strchr(p, ' ')) {
						*e='\0';
						pclose(fp);
						return toString(p);
					}
				}
			}
		}
	}
	pclose(fp);
	return "0.0.0.0";
}

string func::openFile(const char * fileName){
	string line;
	string returnvalue;
	ifstream myfile (fileName);
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			returnvalue += line + "\r\n";
		}
		myfile.close();
		
		return returnvalue;
	}else{
		return "file " + toString(fileName) + " not found!";
	}
	
}

string func::openBinaryFile(const char *fileName, int* fsize)
{
  std::ifstream in(fileName, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    *fsize = in.tellg();
    contents.resize(*fsize);
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  return "";
  //throw(errno);
}
