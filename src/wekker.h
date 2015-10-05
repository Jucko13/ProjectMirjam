#ifndef WEKKER_H
#define WEKKER_H

#include <ctime>
#include <string>

using namespace std;

class Wekker {

public:
	Wekker();
	Wekker(char const* duetimestr, bool repeat, int userdata);
	Wekker(int dueinmin, int userdata);
	virtual ~Wekker();

	void settime(char const* duetimestr);
	void settime(int dueinmin);
	string gettime();
	bool isdue();
	void recalculatetime();
	int getdata();
	bool isrepeating();

private:
	std::tm endtime;
	int data;
	bool repeating;
};

#endif
