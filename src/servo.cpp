#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>

#define NUM_GPIO 32

int run = 1;
int gpio_pin;

void stop(int signum)
{
	run = 0;
}

int main(int argc, char *argv[])
{
	if (gpioInitialise() < 0) return -1;

	gpioSetSignalFunc(SIGINT, stop);

	if (argc == 1) gpio_pin = 4;
	else
	{
		gpio_pin = atoi(argv[1]);
	}

	printf(", control C to stop.\n");
	int last_pos = 1500;
	int new_pos = last_pos;
	while (run)
	{
		std::cout << "new pos: " << new_pos << " last pos " << last_pos << std::endl;
		if (new_pos != last_pos)
		{
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
			last_pos = pos;
		}

		std::cout << "New position? :\n";
		std::cin >> new_pos;
	}

	printf("\ntidying up\n");

	gpioServo(gpio_pin, 0);
	gpioTerminate();

	return 0;
}

