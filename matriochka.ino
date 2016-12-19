/* 
*
* ESO : Estaca Space Odyssey - Space engineering association of Estaca 
* ESTACA : Transportation engineering school in France
*
* Hardware : Arduino 101 
*
* MATRIOCHKA Project
*
*/

#include <Inertial.h>
#include <Switch.h>

Inertial  inertial(13, 10);
Switch    mechanical_switch(8, 5, 7, 1, 3);

unsigned long   dt = 0.0;                 
unsigned long   t_process = 0.0;          

bool  tilt_signal = false;
bool  jack_signal = false;
bool  para_signal = false;
bool  time_signal = false;
bool  prop_signal = false;

int IGNITION = 12;

void setup() {
  Serial.begin(9600);
  pinMode(IGNITION, OUTPUT);
  digitalWrite(IGNITION, LOW);
}

void loop() {
  // Update new values
  t_process = micros();
  inertial.Update(dt);

  // Get all signals
  tilt_signal = inertial.Attitude();
  jack_signal = mechanical_switch.Jack();
  para_signal = mechanical_switch.Para();
  if(jack_signal){
    prop_signal = inertial.Propulsion();
    time_signal = mechanical_switch.Time();
  }
  else{
    prop_signal = false;
    time_signal = false;   
  }

  // Ignition command
  if(tilt_signal && jack_signal && para_signal && time_signal){
    digitalWrite(IGNITION, HIGH);
  }
  else{
    digitalWrite(IGNITION, LOW);    
  }

  // Time process
  dt = micros() - t_process;
}
