#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>

#define NUM_GPIO 32
#define MIN_RANGE 500
#define MAX_RANGE 2500

int run = 1;
int gpio_pin;

void stop(int signum)
{
	run = 0;
}

int rotate_servo(int last_pos, int new_pos)
{
	if (new_pos > MAX_RANGE || new_pos < MIN_RANGE)
	{
		std::cout "Position of " << new_pos << " is invalid. Please select a range between " << MIN_RANGE << " and " << MAX_RANGE << ".\n";
		return last_pos;
	}
	return 0;
	int pos = last_pos;
	int step = 1;
	if (new_pos < pos)
	{
		step = -1;
	}
	while (pos != new_pos)
	{
		printf("%d \n", pos);

		gpioServo(gpio_pin, pos);
		pos += step;
		time_sleep(0.001);
	}
	return new_pos;
}

int main(int argc, char *argv[])
{
	int last_pos = 1500;
	int new_pos = last_pos;

	if (gpioInitialise() < 0)
	{
		std::cout << "Error initializing gpio - exiting\n";
		return -1;
	}

	gpioSetSignalFunc(SIGINT, stop);

	if (argc == 2)
	{
		gpio_pin = atoi(argv[1]);
	}

	printf("Calibration routine starting - control C to stop.\n");
	while (run)
	{
		std::cout << "new pos: " << new_pos << " last pos " << last_pos << std::endl;
		if (new_pos != last_pos)
		{
			last_pos = rotate_servo(last_pos, new_pos);
			//int pos = last_pos;
			//int step = 1;
			//if (new_pos < pos)
			//{
			//	step = -1;
			//}
			//while (pos != new_pos)
			//{
			//	printf("%d \n", pos);

			//	gpioServo(gpio_pin, pos);
			//	pos += step;
			//	time_sleep(0.001);
			//}
			//last_pos = pos;
		}

		std::cout << "New position? :\n";
		std::cin >> new_pos;
	}

	printf("\ntidying up\n");

	gpioServo(gpio_pin, 0);
	gpioTerminate();

	return 0;
}

