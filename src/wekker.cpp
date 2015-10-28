
#include "wekker.h"
#include "functions.h"
#include <ctime>
#include <string>
//#include <iostream>

//using namespace std;

/**
 * @brief creates a timer
 */
Wekker::Wekker() : data(0), repeating(true)
{
	std::time_t currentTime = std::time(NULL);
	endTime = *localtime(&currentTime);
}

/**
 * @brief creates a timer
 * @param time to watch for expiry (hour:minute)
 * @param repeating timer or not
 * @param data for user
 */
Wekker::Wekker(char const* duetimestr, bool repeat, int userdata) : data(userdata), repeating(repeat)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

/**
 * @brief creates a timer
 * @param timeout to watch for expiry (minutes)
 * @param data for user
 */
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

/**
 * @brief modify the due-time
 * @param time to watch for expiry (hour:minute)
 */
void Wekker::setTime(char const* duetimestr)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

/**
 * @brief modify the due-time
 * @param timeout to watch for expiry (minutes)
 */
void Wekker::setTime(int dueinmin)
{
	std::time_t currentTime = std::time(NULL);
	std::tm *currenttm = localtime(&currentTime);

	currenttm->tm_min += dueinmin;
	endTime = *currenttm;
}

/**
 * @brief retrive the due-time
 * @return due-timestring (hour:minute)
 */
std::string Wekker::getTime()
{
	return func::toString<int>(endTime.tm_hour) + ":" + func::toString<int>(endTime.tm_min);
}

/**
 * @brief has the due-time been passed
 * @return true if time has passed
 */
bool Wekker::isDue()
{
	double seconds;
	seconds = difftime(mktime(&endTime), std::time(NULL));

	if (seconds > 0) {
		return false;
	}
	return true;
}

/**
 * @brief set the triggered timer for the next ocurrence of the due-time
 */
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

/**
 * @brief gets the userdata
 * @return data for user
 */
int Wekker::getData()
{
	return data;
}

/**
 * @brief gets if the timer is to be repeating
 * @return needs recalculate
 */
bool Wekker::isRepeating()
{
	return repeating;
}


