#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>
#include <memory>
#include "../include/pigpioServo.h"


int main(int argc, char *argv[])
{
	AngleMaps angle_maps;
	int gpio_pin = 17;

	AngleMap center;
	AngleMap right;
	AngleMap left;

	if (argc == 8)
	{
		gpio_pin = atoi(argv[1]);

		right.angle = atoi(argv[2]);
		right.pulse_width = atoi(argv[3]);

		center.angle = atoi(argv[4]);
		center.pulse_width = atoi(argv[5]);

		left.angle = atoi(argv[6]);
		left.pulse_width = atoi(argv[7]);
		std::cout << "\nGood job!\n";
		std::cout << "center angle is " << center.angle << "and pulse is " << center.pulse_width << std::endl;
	}
	else
	{
		std::cout << "\nYou called me wrong. Call me like this:\n";
		std::cout << "\tsudo ./servo_tester <gpio_pin> <right_angle> <right_pulse> <center_angle> <center_pulse> <left_angle> <left_pulse>\n\n";
		exit(1);
	}

	angle_maps.center_map = center;
	angle_maps.right_map = right;
	angle_maps.left_map = left;

	InitialOffset offset;
	offset.offsetAngle = 90;
	offset.offsetX = 0;
	offset.offsetY = 0;

	std::cout << "\nHERE A" << std::flush;
	std::shared_ptr<pigpioServo> servo = std::make_shared<pigpioServo>(gpio_pin, angle_maps, offset);
	std::cout << "\nHERE B" << std::flush;

	time_sleep(5.0);
	std::cout << "\nTurning servo to 45 degrees...\n" << std::flush;
	servo->TurnToAngle(45);
	time_sleep(2.0);
	std::cout << "\nTurning servo to 135 degrees...\n" << std::flush;
	servo->TurnToAngle(135);
	time_sleep(2.0);


	std::cout << "\nCleaning up...\n";
	return 0;
}

