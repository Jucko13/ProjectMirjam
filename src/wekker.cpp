
#include "wekker.h"
#include "functions.h"
#include <ctime>
#include <string>
//#include <iostream>

//using namespace std;

Wekker::Wekker() : data(0), repeating(true)
{
	std::time_t currentTime = std::time(NULL);
	endTime = *localtime(&currentTime);
}

Wekker::Wekker(char const* duetimestr, bool repeat, int userdata) : data(userdata), repeating(repeat)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

Wekker::Wekker(int dueinmin, int userdata) : data(userdata), repeating(false)
{
	std::time_t currentTime = std::time(NULL);
	std::tm *currenttm = localtime(&currentTime);

	currenttm->tm_min += dueinmin;
	endTime = *currenttm;
}

Wekker::~Wekker()
{
}

void Wekker::setTime(char const* duetimestr)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

void Wekker::setTime(int dueinmin)
{
	std::time_t currentTime = std::time(NULL);
	std::tm *currenttm = localtime(&currentTime);

	currenttm->tm_min += dueinmin;
	endTime = *currenttm;
}

std::string Wekker::getTime()
{
	return func::toString<int>(endTime.tm_hour) + ":" + func::toString<int>(endTime.tm_min);
}

bool Wekker::isDue()
{
	double seconds;
	seconds = difftime(mktime(&endTime), std::time(NULL));

	if (seconds > 0) {
		return false;
	}
	return true;
}

void Wekker::recalculateTime()
{
	double seconds;
	std::time_t currentTime = std::time(NULL);
	std::tm *currenttm = localtime(&currentTime);

	currenttm->tm_hour = endTime.tm_hour;
	currenttm->tm_min = endTime.tm_min;
	//currenttm->tm_sec = endtime.tm_sec;//no seconds required
	currenttm->tm_sec = 0;
	endTime = *currenttm;

	seconds = difftime(mktime(&endTime), currentTime);
	if (seconds < 0) {
		endTime.tm_mday++;//if (targettime > currenttime) it must be te next day
	}
}

int Wekker::getData()
{
	return data;
}

bool Wekker::isRepeating()
{
	return repeating;
}


