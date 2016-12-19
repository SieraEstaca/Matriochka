#include "Arduino.h"
#include "Inertial.h"
#include <CurieIMU.h>
#include <MadgwickAHRS.h>

Madgwick filter;

Inertial::Inertial(int ledProp, int ledTilt)
{	
	// Set inertial class outputs
	trust_signal = false;
	attitude_signal = false;

	// Set Intel Curie IMU parameters
	accelerometerRange = 2;
	gyroRange = 250;
	frequency = 25;
	CurieIMU.begin();
	CurieIMU.setGyroRate(frequency);
	CurieIMU.setAccelerometerRate(frequency);
	filter.begin(frequency);
	CurieIMU.setAccelerometerRange(accelerometerRange); 
	CurieIMU.setGyroRange(gyroRange);
	
	// Set class functions parameters
	ax_filt = 0.0, ay_filt = 0.0, az_filt = 0.0;
	gx_filt = 0.0, gy_filt = 0.0, gz_filt = 0.0;
	g = 9.81, trust = 120.0, isp = 3.7, mass = 12.0, rate = 0.80, sum = 0.0;
	threshold = rate*trust*isp/mass; 
	tilt_filt = 0.0;
	
	// Setup all pins 
	_ledProp = ledProp;
	_ledTilt = ledTilt;
	pinMode(_ledProp, OUTPUT);
	pinMode(_ledTilt, OUTPUT);
	digitalWrite(_ledProp, LOW); 
    digitalWrite(_ledTilt, LOW);
}

void Inertial::Update(float gain)
{
	// Intel Curie datas
	CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);
	_gain = gain/1000000.0;
	
	// Accelerometers values
	ax = (aix * accelerometerRange)/32768.0;
	ax_filt = ax_filt + _gain*(ax-ax_filt);
	ay = (aiy * accelerometerRange)/32768.0;
	ay_filt = ay_filt + _gain*(ay-ay_filt);
	az = (aiz * accelerometerRange)/32768.0;
	az_filt = az_filt + _gain*(az-az_filt);


	// Gyro values
	gx = (gix * gyroRange)/32768.0;
	gx_filt = gx_filt + _gain*(gx-gx_filt);
	gy = (giy * gyroRange)/32768.0;
	gy_filt = gy_filt + _gain*(gy-gy_filt);
	gz = (giz * gyroRange)/32768.0;
	gz_filt = gz_filt + _gain*(gz-gz_filt);
}

bool Inertial::Propulsion(){
	// Momentum calculation
	a = (ax+1)*g;
	sum += a*_gain/1000000.0;
	
	// Enable signal if propulsion system going to shut down
	if (sum >= threshold){
		trust_signal = true;
		digitalWrite(_ledProp, HIGH); 
	}
	else{
		trust_signal = false;	
		digitalWrite(_ledProp, LOW); 		
	}	
	return trust_signal;
}

bool Inertial::Attitude(){
	// Get orientation
	filter.updateIMU(gx_filt, gy_filt, gz_filt, ax_filt, ay_filt, az_filt);
	tilt = filter.getPitch();
	
	// Enable signal if launcher attitude is correct
	if (tilt >= 70.0 and tilt <= 90.0){
		attitude_signal = true;
		digitalWrite(_ledTilt, HIGH); 
	}
	else{
		attitude_signal = false;
		digitalWrite(_ledTilt, LOW); 
	}
	return attitude_signal;
}