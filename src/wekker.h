#ifndef WEKKER_H
#define WEKKER_H

#include <ctime>
#include <string>

//using namespace std;

class Wekker {

public:
	Wekker();
	Wekker(char const* duetimestr, bool repeat, int userdata);
	Wekker(int dueinmin, int userdata);
	virtual ~Wekker();

	void setTime(char const* duetimestr);
	void setTime(int dueinmin);
	std::string getTime();
	bool isDue();
	void recalculateTime();
	int getData();
	bool isRepeating();

private:
	std::tm endTime;
	int data;
	bool repeating;
};

#endif
