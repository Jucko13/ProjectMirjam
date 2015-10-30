#include "buzzer.h"
#include "arduPi.h"


Buzzer::Buzzer(int p) : pin(p)
{
	pinMode(pin, OUTPUT);
}

void Buzzer::playAlarm(){
	for(int i = 0; i < 5000; i++){
		digitalWrite(pin, HIGH);
		delayMicroseconds(100);
		digitalWrite(pin, LOW);
		delayMicroseconds(100);
	}
}
