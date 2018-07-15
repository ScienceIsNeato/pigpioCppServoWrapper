g++ -Wall -pthread -o calibrate src/calibrate.cpp -lpigpio -lrt
g++ -Wall -pthread -o servo_tester src/pigpioServo.cpp src/servo_tester.cpp -lpigpio -lrt
