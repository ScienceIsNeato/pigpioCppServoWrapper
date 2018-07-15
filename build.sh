echo "Building calibration binary..."
g++ -Wall -pthread -o calibrate src/calibrate.cpp -lpigpio -lrt
echo "Successfully built 'calibrate'"

echo "Building test binary..."
g++ -Wall -pthread -o servo_tester src/pigpioServo.cpp src/servo_tester.cpp -lpigpio -lrt
echo "Successfully built 'servo_tester'"
