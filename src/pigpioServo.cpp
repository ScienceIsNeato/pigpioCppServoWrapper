#include "../include/pigpioServo.h"

pigpioServo::pigpioServo()
{

}

pigpioServo::pigpioServo(AngleMap max_left, AngleMap center, AngleMap max_right)
{
	_max_left = max_left;
	_center = center;
	_max_right = max_right;
}

pigpioServo::pigpioServo(AngleMap max_left, AngleMap center, AngleMap max_right, InitialOffset initial_offset)
{
	_max_left = max_left;
	_center = center;
	_max_right = max_right;
	_initial_offset = initial_offset;
}

pigpioServo::~pigpioServo()
{
	Stop();
}

void pigpioServo::Initialize()
{

}

void pigpioServo::Stop()
{

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
