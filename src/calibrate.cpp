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

bool ConfirmVal(int test_val, int position, int &accepted_val)
{
	int new_pos = test_val;
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
		return ConfirmVal(new_pos, position, accepted_val);
	}
	else
	{
		accepted_val = new_pos;
		return true;
	}
	return false;
}

int PrintPrompt(int position)
{
	int val;
	std::string pos_string = GetPositionString(position);

	std::cout << "Enter a test value for the " << pos_string << " - value: ";
	std::cin >> val;
	return val;
}

int GetVal(int position)
{
	int accepted_value;
	while (true)
	{
		int test_val = PrintPrompt(position);
		if (ConfirmVal(test_val, position, accepted_value))
		{
			return accepted_value;
		}
	}
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
	int center_val = GetVal(CENTER);
	int right_val = GetVal(RIGHT);
	int left_val = GetVal(LEFT);

	std::cout << "CALIBRATION RESULTS FOR GPIO: " << gpio_pin << std::endl;
	std::cout << "Right Calibration Value:\t" << right_val << ", Center:\t" << center_val << ", Left:\t" << left_val << std::endl;
	std::cout << "(TODO - add instructions for what to do with these values...)\n";

	gpioServo(gpio_pin, 0);
	gpioTerminate();

	return 0;
}

