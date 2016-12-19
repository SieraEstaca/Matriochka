#ifndef Switch_h
#define Switch_h

#include "Arduino.h"

class Switch
{
  public:
	// Constructor
    Switch(int ledTime, int ledJack, int ledPara, int JACK, int PARA);
	// Functions
	bool Time();
    bool Jack();
	bool Para();
	
  private:
	int _ledTime, _ledJack, _ledPara;
	int _JACK, _PARA;
	bool time_signal, jack_signal, para_signal;
	bool print_counter_1, print_counter_2, print_counter_3;
	unsigned long initial_time, current_time, elapsed_time, timerMin, timerMax;
};

#endif