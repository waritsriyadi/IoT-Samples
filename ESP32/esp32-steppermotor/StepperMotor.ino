#include <Stepper.h>

// Define the number of steps per revolution
const int stepsPerRevolution = 256;

// Define the motor control pins
const int motorPin1 = 12;
const int motorPin2 = 13;
const int motorPin3 = 14;
const int motorPin4 = 27;

// Initialize the stepper library on the motor control pins
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  // Set the speed of the motor
  myStepper.setSpeed(32); // RPM
}

void loop() {
  // Step one revolution in one direction
  myStepper.step(stepsPerRevolution);
  delay(500);

  // Step one revolution in the other direction
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
