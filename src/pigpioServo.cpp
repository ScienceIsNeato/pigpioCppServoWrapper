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
	std::cout << "FUCKING DESTRUCTOR GETTING CALLED\n" << std::flush;
	//Stop();
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
	_last_pos = _center.pulse_width;
	std::cout << "A gpio pin is " << _gpio_pin << " and _last_pos is " << _last_pos << std::flush;
	if (gpioInitialise() < 0)
	{
		std::cout << "Error initializing gpio.\n" << std::flush;
		return false;
	}

	std::cout << "gpioInitialise() was successful\n" << std::flush;
	std::cout << "B gpio pin is " << _gpio_pin << " and _last_pos is " << _last_pos << std::flush;

	// TODO replace with turn call
	TurnToAngle(90);
	//gpioServo(_gpio_pin, _center.pulse_width);
	std::cout << "about to set _last_pos to " << _center.pulse_width << std::flush;
	_last_pos = _center.pulse_width;
	return true;
}

void pigpioServo::Stop()
{
	gpioServo(_gpio_pin, 0);
	std::cout << "In STOP - about to terminate!\n" << std::flush;
	gpioTerminate();
}

void pigpioServo::TurnToAngle(double angle)
{
	gpioInitialise(); // TODO find out why this has to be here
	_last_pos = 1600;
	if(!IsAngleValid(angle))
	{
		std::cout << "You entered and invalid angle, dummy.\n" << std::flush;
		return;
	}

	int new_pos = AngleToPulseWidth(angle);
	int pos = _last_pos;
	int step = 1;

	std::cout << "Last pos is " << _last_pos << " new pos is " << new_pos << std::flush;
	// check valid range
	if (new_pos > MAX_RANGE || new_pos < MIN_RANGE)
	{
		std::cout << "Position of " << new_pos << " is invalid. Please select a range between " << MIN_RANGE << " and " << MAX_RANGE << ".\n" << std::flush;
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
	std::cout << "IM IN HERE about to set last_pos to " << new_pos << std::flush;
	_last_pos = new_pos;
}

bool pigpioServo::IsAngleValid(double angle)
{
	return true;
}

int pigpioServo::AngleToPulseWidth(double angle)
{
	if (angle > _center.angle)
	{
		std::cout << "returning " << _max_left.pulse_width << std::flush;
		return _max_left.pulse_width;
	}
	else
	{
		std::cout << "returning " << _max_right.pulse_width << std::flush;
		return _max_right.pulse_width;
	}
}
