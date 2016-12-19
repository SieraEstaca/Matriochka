#ifndef Inertial_h
#define Inertial_h

#include "Arduino.h"
#include <CurieIMU.h>
#include <MadgwickAHRS.h>

class Inertial
{
  public:
	// Constructor
    Inertial(int ledProp, int ledTilt);
	// Functions
	void Update(float gain);
    bool Propulsion();
	bool Attitude();
	
  private:
	// Constructor
	bool trust_signal, attitude_signal;
  	int accelerometerRange, gyroRange, frequency;
	int _ledProp, _ledTilt;
    // Update
	int   aix, aiy, aiz, gix, giy, giz;
	float ax, ay, az, gx, gy, gz, _gain;
	float ax_filt, ay_filt, az_filt, gx_filt, gy_filt, gz_filt;
	// Trust parameters
	float g, a, trust, isp, mass, rate, threshold, sum;
	// Attitude parameters
	float tilt, tilt_filt;
};

#endif