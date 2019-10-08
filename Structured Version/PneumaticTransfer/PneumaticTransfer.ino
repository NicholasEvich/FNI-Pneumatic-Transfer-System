#include "StepMotor.h"

void setup() {
  Serial.begin(9600);
  // All setup is done in the StepMotor constructor
  // Seeding the random number generator for DP valve homing
  //    Pin A0 is referenced because it is not used, and thus an analog read of it will 
  //    almost certainly be different every time
  // Seeding the random function for randomizing the valve position
  randomSeed(analogRead(A0));
}

void loop() {
  // Create a StepMotor object and home the rotary valve
  StepMotor* Motor = new StepMotor;

  // Wait for the sample send button to be pressed, then turn the vacuum pump on and send the sample
  while (digitalRead(Motor->getSendPin()) == LOW) {
    // wait
  }
  Serial.print("Send button pressed, starting motor and sending sample in ");
  for (int i = 2; i > 0; i--) {
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  Serial.print("\n");
  Motor->sendSample();

  // Wait for the sample return button to be pressed, then turn the vacuum pump on and return the sample
  while (digitalRead(Motor->getReturnPin()) == LOW) {
    // wait
  }
  Serial.print("Return button pressed, starting motor and returning sample in ");
  for (int i = 2; i > 0; i--) {
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  Serial.print("\n");
  Motor->returnSample();

  // Clear up dynamic memory for the next sample
  delete Motor;
  Motor = NULL;
}
