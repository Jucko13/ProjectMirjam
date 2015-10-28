
#include "wekker.h"
#include "functions.h"
#include <ctime>
#include <string>
//#include <iostream>

//using namespace std;

/**
 * @brief Wekker::Wekker
 */
Wekker::Wekker() : data(0), repeating(true)
{
	std::time_t currentTime = std::time(NULL);
	endTime = *localtime(&currentTime);
}

/**
 * @brief Wekker::Wekker
 * @param duetimestr
 * @param repeat
 * @param userdata
 */
Wekker::Wekker(char const* duetimestr, bool repeat, int userdata) : data(userdata), repeating(repeat)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

/**
 * @brief Wekker::Wekker
 * @param dueinmin
 * @param userdata
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
 * @brief Wekker::setTime
 * @param duetimestr
 */
void Wekker::setTime(char const* duetimestr)
{
	strptime(duetimestr, "%H:%M", &endTime);
	recalculateTime();
}

/**
 * @brief Wekker::setTime
 * @param dueinmin
 */
void Wekker::setTime(int dueinmin)
{
	std::time_t currentTime = std::time(NULL);
	std::tm *currenttm = localtime(&currentTime);

	currenttm->tm_min += dueinmin;
	endTime = *currenttm;
}

/**
 * @brief Wekker::getTime
 * @return
 */
std::string Wekker::getTime()
{
	return func::toString<int>(endTime.tm_hour) + ":" + func::toString<int>(endTime.tm_min);
}

/**
 * @brief Wekker::isDue
 * @return
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
 * @brief Wekker::recalculateTime
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
 * @brief Wekker::getData
 * @return
 */
int Wekker::getData()
{
	return data;
}

/**
 * @brief Wekker::isRepeating
 * @return
 */
bool Wekker::isRepeating()
{
	return repeating;
}


