#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>

#define NUM_GPIO 32
#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2500
#define DEFAULT_CENTER 1500

// Used when initializing the object if there is a distance or angle offset
// between the servo and the sensor point
struct InitialOffset
{
	double offsetX; // x distance between servo and sensor
	double offsetY; // y distance between servo and sensor
	double offsetAngle; // angle offset between servo and sensor
};

// A way to map an angle in degrees to a pulsewidth
struct AngleMap
{
	double angle;
	int pulse_width;
};

class pigpioServo
{
public:
	pigpioServo();
	pigpioServo(AngleMap max_left, AngleMap center, AngleMap max_right);
	pigpioServo(AngleMap max_left, AngleMap center, AngleMap max_right, InitialOffset initial_offset);
	~pigpioServo();
	void Initialize();
	void Stop();
	void TurnToAngle(double angle);
	bool IsAngleValid(double angle);

private:
	InitialOffset _initial_offset;
	int _gpio_pin;
	AngleMap _max_left;
	AngleMap _center;
	AngleMap _max_right;
	int default_pulsewidth;
	int current_pulsewidth;

	int AngleToPulseWidth(double angle);
};