#include "../include/pigpioServo.h"

pigpioServo::pigpioServo(int gpio_pin, AngleMaps boundaries)
{
	// create empty offset
	InitialOffset offset;
	offset.offsetAngle = 90;
	offset.offsetX = 0;
	offset.offsetY = 0;

	// call default constructor
	pigpioServo servo = pigpioServo(gpio_pin, boundaries, offset);
}

pigpioServo::pigpioServo(int gpio_pin, AngleMaps boundaries, InitialOffset initial_offset)
{
	SetGpioPin(gpio_pin);
	SetBoundaries(boundaries);
	SetOffset(initial_offset);
	Initialize();
}

pigpioServo::~pigpioServo()
{
	Stop();
}

void pigpioServo::SetBoundaries(AngleMaps boundaries)
{
	_max_left = boundaries.left_map;
	_center = boundaries.center_map;
	_max_right = boundaries.right_map;
}

void pigpioServo::SetOffset(InitialOffset offset)
{
	_initial_offset = offset;
}

void pigpioServo::SetGpioPin(int pin)
{
	_gpio_pin = pin;
}

bool pigpioServo::Initialize()
{
	if (gpioInitialise() < 0)
	{
		std::cout << "Error initializing gpio.\n";
		return false;
	}

	std::cout << "gpioInitialise() was successful\n";

	// TODO replace with turn call
	//gpioServo(_gpio_pin, _center.pulse_width);
	std::cout << "about to set _last_pos to " << _center.pulse_width << std::endl;
	_last_pos = _center.pulse_width;
	return true;
}

void pigpioServo::Stop()
{
	gpioServo(_gpio_pin, 0);
	gpioTerminate();
}

void pigpioServo::TurnToAngle(double angle)
{
	gpioInitialise(); // TODO find out why this has to be here
	if(!IsAngleValid(angle))
	{
		std::cout << "You entered and invalid angle, dummy.\n";
		return;
	}

	int new_pos = AngleToPulseWidth(angle);
	int pos = _last_pos;
	int step = 1;

	std::cout << "Last pos is " << _last_pos << " new pos is " << new_pos << std::endl;
	// check valid range
	if (new_pos > MAX_RANGE || new_pos < MIN_RANGE)
	{
		std::cout << "Position of " << new_pos << " is invalid. Please select a range between " << MIN_RANGE << " and " << MAX_RANGE << ".\n";
		return;
	}

	// sweeping up or down?
	if (new_pos < pos)
	{
		step = -1;
	}

	// Perform the rotation
	while (pos != new_pos)
	{
		//printf("%d \n", pos);

		gpioServo(_gpio_pin, pos);
		pos += step;
		time_sleep(0.001);
	}
	_last_pos = new_pos;
}

bool pigpioServo::IsAngleValid(double angle)
{
	return true;
}

int pigpioServo::AngleToPulseWidth(double angle)
{
	return 1490;
}
