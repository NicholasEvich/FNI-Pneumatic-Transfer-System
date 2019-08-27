#include "StepMotor.h"

void setup() {
  // All setup is done in the StepMotor constructor
}

void loop() {
  // Create a StepMotor object and home the rotary valve
  StepMotor* Motor = new StepMotor;
  Motor->homeValve();

  // Wait for the sample send button to be pressed, then turn the vacuum pump on and send the sample
  while (digitalRead(Motor->getSendPin()) == LOW) {
    // wait
  }
  Motor->sendSample();

  // Wait for the sample return button to be pressed, then turn the vacuum pump on and return the sample
  while (digitalRead(Motor->getReturnPin()) == LOW) {
    // wait
  }
  Motor->returnSample();

  // Clear up dynamic memory for the next sample
  delete Motor;
  Motor = null;
}
