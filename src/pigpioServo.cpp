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
	}

	// TODO replace with turn call
	gpioServo(_gpio_pin, _center.pulse_width);
}

void pigpioServo::Stop()
{
	gpioServo(_gpio_pin, 0);
	gpioTerminate();
}

void pigpioServo::TurnToAngle(double angle)
{

}

bool pigpioServo::IsAngleValid(double angle)
{
	return false;
}

int pigpioServo::AngleToPulseWidth(double angle)
{
	return 0;
}
