// ----------------Stepper Motor Controller Reference--------------------
// https://www.omc-stepperonline.com/download/DM542T.pdf
// ----------------Stepper Motor Power Supply Reference------------------
// https://cpi.com.ar/info_productos/pdf/S8VKG.pdf
// ---------------------Stepper Motor Reference--------------------------
// https://openbuildspartstore.com/nema-23-stepper-motor/
// ----------------Photoelectric Sensor Reference------------------------
// https://cdn.automationdirect.com/static/specs/pefiberdft.pdf
// ----------------------------------------------------------------------

#include <Arduino.h>
#include <StepMotor.h>
 
StepMotor::StepMotor() { //initializer list?
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
void StepMotor::randomPos() {
    Serial.print("Generating a random starting position before homing sequence executes\n");
    for (int i = 0; i < random(50, 150); i++) {
    digitalWrite(getPulsePin(), HIGH);
    delay(25);
    digitalWrite(getPulsePin(), LOW);
    delay(25);
    }
    delay(1000);
}

void StepMotor::microstep(float deg) {
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

void StepMotor::homeValve() {
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
}

void StepMotor::sendSample() {
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

void StepMotor::returnSample() {
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

//Getter methods aren't really needed, as none of these functions should be used outside of the class's own functions
byte StepMotor::getPulsePin() {
    return PULSE_PIN;
}

byte StepMotor::getEnabPin() {
    return ENAB_PIN;
}

byte StepMotor::getDirPin() {
    return DIR_PIN;
}

byte StepMotor::getRelayPin() {
    return VACUUM_RELAY;
}

byte StepMotor::getSendPin() {
    return BUTTON_SEND;
}

byte StepMotor::getReturnPin() {
    return BUTTON_RETURN;
}

byte StepMotor::getHomePin() {
    return HOMING_SENSOR;
}

StepMotor::~StepMotor() {
    //clear pins/set them all to low voltage?
}
