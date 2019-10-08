// To do:
// Make this code robust so that the microsteps per revolution do not matter and are coded into a variable
// then a scaling factor can be applied anywhere a number of steps is referenced

#define MICROSTEPS 400

class StepMotor {

public:
    StepMotor() {
      pinMode(getPulsePin(), OUTPUT);
      pinMode(getEnabPin(), OUTPUT);
      pinMode(getDirPin(), OUTPUT);
      pinMode(getRelayPin(), OUTPUT);
      pinMode(getSendPin(), INPUT);
      pinMode(getReturnPin(), INPUT);
      pinMode(getHomePin(), INPUT);

      randomPos();
      homeValve();
    }

    // The purpose of this function is to start the valve at a random (not homed) position
    //    so that the homing function works consistently each time
    // If the valve starts at a "homed" position (of which there are many), then consistency
    //    will not exist
    void randomPos() {
      Serial.print("Generating a random starting position before homing sequence executes\n");
      for (int i = 0; i < random(50, 150); i++) {
        digitalWrite(getPulsePin(), HIGH);
        delay(25);
        digitalWrite(getPulsePin(), LOW);
        delay(25);
      }
      delay(1000);
    }
    
    void microstep(float deg) {
      int steps = static_cast<int>(MICROSTEPS*(deg/360.0));
      Serial.print("Stepping now\n");
      for (int stepCounter = 0; stepCounter < steps; stepCounter++) {
        digitalWrite(getPulsePin(), HIGH);
        delay(10);
        digitalWrite(getPulsePin(), LOW);
        delay(10);    
        Serial.print("Step\n");    
      }
    }

    void homeValve() {
      Serial.print("Starting to home valve\n");
      while (analogRead(getHomePin()) < 75) {
        digitalWrite(getPulsePin(), HIGH);
        delay(50);
        digitalWrite(getPulsePin(), LOW);
        delay(50);
      }

      // Hardcoded fix for valve fabrication imperfections
      for (int i = 0; i < 11; i++) {
        digitalWrite(getPulsePin(), HIGH);
        delay(100);
        digitalWrite(getPulsePin(), LOW);
        delay(100);
      }
      
      Serial.print("Homing of DP Valve Complete\n");

      //----- For Testing Purposes Only ---------------
      // digitalWrite(getRelayPin(), HIGH);
    }

    void sendSample() {
      // Turning the vacuum pump on through a solid-state relay
      digitalWrite(getRelayPin(), HIGH);
      Serial.print("Vacuum pump started\n");
      // Waiting for the pump motor to reach full speed
      delay(5000);
      Serial.print("Vacuum pump at full power, sending sample now\n");

      // Enabling the stepper motor. This step is for redundancy
      // digitalWrite(getEnabPin(), LOW);
  
      // The HIGH direction corresponds to clockwise
      digitalWrite(getDirPin(), HIGH);
  
      // Step the motor 67.5 degrees to send the sample
      microstep(67.5); //for whatever reason I am not getting any suction here (the motor is not stepping)
  
      // Wait a set time for the sample to reach the end of the tube (this section of the code is subject to change)
      delayMicroseconds(100);
  
      /* Pseudocode for what will actually happen at this point:
      - Step the motor back a little bit (call microstep function)
      - Slow the sample to a stop
      - Microstep again (by slightly opening the valve the opposite direction and shoving it in the end of the tube)
      */
  
      // Shut the motor off
      digitalWrite(getRelayPin(), LOW);
      delay(10000);
  
      // Return the direction pin to its initial value
      // digitalWrite(getDirPin(), LOW); 
  
      // Home valve before the next sample send/return so that there is a minimal wait time between command issue and execution
      randomPos();
      homeValve();
    }

    void returnSample() {
      // Turning the vacuum pump on through a solid-state relay
      digitalWrite(getRelayPin(), HIGH);
      Serial.print("Vacuum pump started\n");
      // Waiting for the pump motor to reach full speed
      delay(5000);
      Serial.print("Vacuum pump at full power, sending sample now\n");
      // Enabling the stepper motor. This step is for redundancy
      // digitalWrite(getEnabPin(), LOW);
  
      // The LOW direction corresponds to counterclockwise
      digitalWrite(getDirPin(), LOW); // note the direction
  
      // Step the motor 67.5 degrees to return the sample
      microstep(67.5);
  
      // Wait a set time for the sample to reach the end of the tube (this section of the code is subject to change)
      delayMicroseconds(100);
  
      /* Pseudocode for what will actually happen at this point:
      - Step the motor back a little bit (call microstep function)
      - Slow the sample to a stop
      - Microstep again (by slightly opening the valve the opposite direction and shoving it in the end of the tube)
      */
  
      // Shut the motor off
      digitalWrite(getRelayPin(), LOW);
      delay(10000);
  
      // No need to randomize and home valve, as this will happen at the beginning of the next
      // loop iteration at the creation of a new StepMotor object
    }

    // Getters
    byte getPulsePin() {
      return PULSE_PIN;
    }
    byte getEnabPin() {
      return ENAB_PIN;  
    }
    byte getDirPin() {
      return DIR_PIN;
    }
    byte getRelayPin() {
      return VACUUM_RELAY;
    }
    byte getSendPin() {
      return BUTTON_SEND;
    }
    byte getReturnPin() {
      return BUTTON_RETURN;
    }
    byte getHomePin() {
      return HOMING_SENSOR;
    }

    ~StepMotor() {
      
    }

private:
    // Output pin for pulsing the stepper motor
    const byte PULSE_PIN = 44;
    // Output pin for setting the direction of rotation for the stepper motor
    const byte DIR_PIN = 42;
    // Output pin for enabling the stepper motor
    const byte ENAB_PIN = 40;
    // Output pin for switching the solid-state relay
    const byte VACUUM_RELAY = 50;
    // Input pin for sending samples down into the Fast Neutron Irradiator
    const byte BUTTON_SEND = 30;
    // Input pin for returning samples from the Fast Neutron Irradiator
    const byte BUTTON_RETURN = 32;
    // Input pin for the photoelectric amplifier
    const byte HOMING_SENSOR = A1;
};

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
