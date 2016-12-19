#include "Arduino.h"
#include "Switch.h"

Switch::Switch(int ledTime, int ledJack, int ledPara, int JACK, int PARA)
{	
	// Setup all pins
	_ledTime = ledTime;
	_ledJack = ledJack;
	_ledPara = ledPara;
	_JACK	= JACK;
	_PARA	= PARA;
	pinMode(_ledTime, OUTPUT);
	pinMode(_ledJack, OUTPUT);
	pinMode(_ledPara, OUTPUT);
	pinMode(_JACK	, INPUT	);
	pinMode(_PARA	, INPUT	);
	digitalWrite(_ledTime, LOW); 
    digitalWrite(_ledJack, LOW);
	digitalWrite(_ledPara, LOW);
	
	// Setup all function parameters
	time_signal = false;
	jack_signal = false;
	para_signal = false;
	print_counter_1 = true;
	print_counter_2 = true;
	print_counter_3 = true;
	// Timer
	initial_time = 0;          
	current_time = 0; 
	elapsed_time = 0;             
	timerMin = 3;   
	timerMax = 15;             
}

bool Switch::Time()
{
  // Check time passed
  current_time = millis()/1000;
  elapsed_time = current_time - initial_time;
  
  // Start to send signal
  if(elapsed_time >= timerMin && elapsed_time <= timerMax){
     time_signal = true;
     digitalWrite(_ledTime, HIGH);
     // Print one time in monitor
     if(print_counter_2 == true){
        Serial.println("Timer signal become HIGH");
        Serial.println(current_time);
        print_counter_2 = false;
     } 
  }
   
   // Time is out, signal finished
  if(elapsed_time >= timerMax || elapsed_time <= timerMin){
    time_signal = false;
    digitalWrite(_ledTime, LOW);
    // Print one time in monitor
    if(print_counter_3 == true && elapsed_time >= timerMax){
		Serial.println("Timer out ...");
        Serial.println(current_time);
        print_counter_3 = false;
    }
   }
   
   return time_signal;
}

bool Switch::Jack(){
  if(digitalRead(_JACK)==LOW){
    jack_signal = true;
    digitalWrite(_ledJack, HIGH);
    if(print_counter_1){
      initial_time = millis()/1000;
      Serial.println("Timer start !");
      Serial.println(initial_time);
      print_counter_1 = false;
     }
  }
  
  else
  {
    jack_signal = false; 
    digitalWrite(_ledJack, LOW);	
  }
  
  return jack_signal;
}

bool Switch::Para(){
  if(digitalRead(_PARA)==HIGH){
    para_signal = true;
	digitalWrite(_ledPara, HIGH);
  }
  
  else{
    para_signal = false;
	digitalWrite(_ledPara, LOW);
  }
  
  return para_signal;
}