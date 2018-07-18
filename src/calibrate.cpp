#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>

#define NUM_GPIO 32
#define MIN_RANGE 500
#define MAX_RANGE 2500

#define CENTER 0
#define RIGHT 1
#define LEFT 2

int run = 1;
int gpio_pin;
int last_pos = 1500; // default center value

struct AngleMap
{
	int angle;
	int pulse_width;
};

void stop(int signum)
{
	run = 0;
}

int rotate_servo(int last_pos, int new_pos)
{
	int pos = last_pos;
	int step = 1;

	// check valid range
	if (new_pos > MAX_RANGE || new_pos < MIN_RANGE)
	{
		std::cout << "Position of " << new_pos << " is invalid. Please select a range between " << MIN_RANGE << " and " << MAX_RANGE << ".\n";
		return last_pos;
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

		gpioServo(gpio_pin, pos);
		pos += step;
		time_sleep(0.001);
	}
	return new_pos;
}

std::string GetPositionString(int pos)
{
	std::string pos_string = "center";
	if (pos == LEFT)
	{
		pos_string = "leftmost";
	}
	else if (pos == RIGHT)
	{
		pos_string = "rightmost";
	}

	return pos_string;
}

AngleMap GetAcceptedVal(int position, AngleMap &accepted_val)
{
	int new_pos = accepted_val.pulse_width;
	std::cout << "new pos: " << new_pos << " last pos " << last_pos << std::endl;
	if (new_pos != last_pos)
	{
		last_pos = rotate_servo(last_pos, new_pos);
	}

	std::cout << "Do you accept the value of " << last_pos << " as the value of " << GetPositionString(position) << "? (y/n) -->";
	std::string resp = "n";
	std::cin >> resp;
	if (resp != "y")
	{
		std::cout << "Ok Enter a new value for position <" << GetPositionString(position) << ">: ";
		std::cin >> new_pos;
		accepted_val.pulse_width = new_pos;
		return GetAcceptedVal(position, accepted_val);
	}
	else
	{
		accepted_val.pulse_width = new_pos;
		return accepted_val;
	}
}

AngleMap PrintPrompt(int position)
{
	int angle;
	int pulse_width;
	std::string pos_string = GetPositionString(position);

	std::cout << "Enter the desired angle for the " << pos_string << " value: ";
	std::cin >> angle;

	std::cout << "Enter a test pulse width for " << angle << " degrees: ";
	AngleMap angle_map= { angle, pulse_width };
	return angle_map;
}

AngleMap GetVal(int position)
{
	AngleMap angle_map = PrintPrompt(position);
	return GetAcceptedVal(position, angle_map);
}


int main(int argc, char *argv[])
{
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

	gpioServo(gpio_pin, 1500);

	printf("Calibration routine starting - control C to stop.\n");
	AngleMap center_val = GetVal(CENTER);
	AngleMap right_val = GetVal(RIGHT);
	AngleMap left_val = GetVal(LEFT);

	std::cout << "CALIBRATION RESULTS FOR GPIO: (angle, pulse width)" << gpio_pin << std::endl;
	std::cout << "Right\t(" << right_val.angle << " degrees, " << right_val.pulse_width << " duty cycle)\n";
	std::cout << "Center\t(" << center_val.angle << " degrees, " << center_val.pulse_width << " duty cycle)\n";
	std::cout << "Left\t(" << left_val.angle << " degrees, " << left_val.pulse_width << " duty cycle)\n";

	std::cout << "(TODO - add instructions for what to do with these values...)\n";

	gpioServo(gpio_pin, 0);
	gpioTerminate();

	return 0;
}

