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

#ifndef STEPMOTOR_H
#define STEPMOTOR_H

class StepMotor {

public:
    StepMotor(); 

    void randomPos();

    void microstep(float);

    void homeValve();

    void sendSample();

    void returnSample();

    // Getters
    byte getPulsePin();
    byte getEnabPin();
    byte getDirPin();
    byte getRelayPin();
    byte getSendPin();
    byte getReturnPin();
    byte getHomePin();

    ~StepMotor();

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

#endif
