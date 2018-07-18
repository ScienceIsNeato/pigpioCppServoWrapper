#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>
#include <iostream>
#include "../include/pigpioServo.h"


int main(int argc, char *argv[])
{
	AngleMaps angle_maps;

	AngleMap center;
	AngleMap right;
	AngleMap left;

	angle_maps.center_map = center;
	angle_maps.right_map = right;
	angle_maps.left_map = left;

	pigpioServo *servo = new pigpioServo(angle_maps);
	return 0;
}

