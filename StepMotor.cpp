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

/* TO-DO
figure out how to do classes with arduino
figure out the interrupts for the buttons and the amplifier: these interrupts (or while loop) are the reasons I need getters for all these members
figure out the "this" issue
*/
 
StepMotor::StepMotor() { //initializer list?
    pinMode(getPulsePin(), OUTPUT);
    pinMode(getEnabPin(), OUTPUT);
    pinMode(getDirPin(), OUTPUT);
    pinMode(getRelayPin(), OUTPUT);
    pinMode(getSendPin(), INPUT);
    pinMode(getReturnPin(), INPUT);
    pinMode(getHomePin(), INPUT);
}

void StepMotor::microstep(int steps) {
    for (int stepCounter = 0; stepCounter <= steps; stepCounter++) {
        digitalWriteFast(getPulsePin(), HIGH);
        delayMicroseconds(250);
        digitalWriteFast(getPulsePin(), LOW);
        delayMicroseconds(250);        
    }
}

void StepMotor::homeValve() {
    while (digitalRead(getHomePin()) == LOW) {
        digitalWrite(getPulsePin(), HIGH);
        delay(100);
        digitalWrite(getPulsePin(), LOW);
        delay(100);
    }
}

// Possibly combine the send and return functions into one with one paramter, a boolean for the direction

void StepMotor::sendSample() {
    // Turning the vacuum pump on through a solid-state relay
    digitalWrite(getRelayPin(), HIGH);
    // Waiting for the pump motor to reach full speed
    delay(5000);

    // Enabling the stepper motor. This step is for redundancy
    digitalWriteFast(getEnabPin(), LOW);

    // The HIGH direction corresponds to _______
    digitalWriteFast(getDirPin(), HIGH); // note the direction

    // Step the motor ____ degrees to send the sample
    microstep(100);

    // Wait a set time for the sample to reach the end of the tube (this section of the code is subject to change)
    delay(250);

    /* Pseudocode for what will actually happen at this point:
    - Step the motor back a little bit (call microstep function)
    - Slow the sample to a stop
    - Microstep again (by slightly opening the valve the opposite direction and shoving it in the end of the tube)
    */

    // Shut the motor off
    digitalWrite(getRelayPin(), LOW);

    // Return the direction pin to its initial value
    digitalWrite(getDirPin(), LOW);

    // Home valve before the next sample send/return so that there is a minimal wait time between command issue and execution
    homeValve();
}

void StepMotor::returnSample() {
    // Turning the vacuum pump on through a solid-state relay
    digitalWrite(getRelayPin(), HIGH);
    // Waiting for the pump motor to reach full speed
    delay(5000);

    // Enabling the stepper motor. This step is for redundancy
    digitalWriteFast(getEnabPin(), LOW);

    // The LOW direction corresponds to _______
    digitalWriteFast(getDirPin(), LOW); // note the direction

    // Step the motor ____ degrees to send the sample
    microstep(100);

    // Wait a set time for the sample to reach the end of the tube (this section of the code is subject to change)
    delay(250);

    /* Pseudocode for what will actually happen at this point:
    - Step the motor back a little bit (call microstep function)
    - Slow the sample to a stop
    - Microstep again (by slightly opening the valve the opposite direction and shoving it in the end of the tube)
    */

    // Shut the motor off
    digitalWrite(getRelayPin(), LOW);

    // Return the direction pin to its initial value
    digitalWrite(getDirPin(), LOW);

    // Home valve before the next sample send/return so that there is a minimal wait time between command issue and execution
    homeValve();
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

