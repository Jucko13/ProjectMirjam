
#include "wekker.h"
#include "functions.h"
#include <ctime>
#include <string>
//#include <iostream>

using namespace std;

Wekker::Wekker() : data(0), repeating(true)
{
	std::time_t currenttime = std::time(NULL);
	endtime = *localtime(&currenttime);
}

Wekker::Wekker(char const* duetimestr, bool repeat, int userdata) : data(userdata), repeating(repeat)
{
	strptime(duetimestr, "%H:%M", &endtime);
	recalculatetime();
}

Wekker::Wekker(int dueinmin, int userdata) : data(userdata), repeating(false)
{
	std::time_t currenttime = std::time(NULL);
	std::tm *currenttm = localtime(&currenttime);

	currenttm->tm_min += dueinmin;
	endtime = *currenttm;
}

Wekker::~Wekker()
{
}

void Wekker::settime(char const* duetimestr)
{
	strptime(duetimestr, "%H:%M", &endtime);
	recalculatetime();
}

void Wekker::settime(int dueinmin)
{
	std::time_t currenttime = std::time(NULL);
	std::tm *currenttm = localtime(&currenttime);

	currenttm->tm_min += dueinmin;
	endtime = *currenttm;
}

string Wekker::gettime()
{
	return func::toString<int>(endtime.tm_hour) + ":" + func::toString<int>(endtime.tm_min);
}

bool Wekker::isdue()
{
	double seconds;
	seconds = difftime(mktime(&endtime), std::time(NULL));

	if (seconds > 0) {
		return false;
	}
	return true;
}

void Wekker::recalculatetime()
{
	double seconds;
	std::time_t currenttime = std::time(NULL);
	std::tm *currenttm = localtime(&currenttime);
	//std::tm currenttm2;//-vs2013
	//localtime_s(&currenttm2, &currenttime);//-vs2013
	//std::tm* currenttm = &currenttm2;//-vs2013

	currenttm->tm_hour = endtime.tm_hour;
	currenttm->tm_min = endtime.tm_min;
	//currenttm->tm_sec = endtime.tm_sec;//no seconds required
	currenttm->tm_sec = 0;
	endtime = *currenttm;

	seconds = difftime(mktime(&endtime), currenttime);
	if (seconds < 0) {
		endtime.tm_mday++;//if (targettime > currenttime) it must be te next day
	}
}

int Wekker::getdata()
{
	return data;
}

bool Wekker::isrepeating()
{
	return repeating;
}


