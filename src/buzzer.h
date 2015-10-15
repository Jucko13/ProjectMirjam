#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
public:
	Buzzer(int);
	void playAlarm();
private:
	int pin;
};

#endif //BUZZER_H


