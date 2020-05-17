#ifndef __STEPPER_H__
#define __STEPPER_H__
#include <Arduino.h>

class STEPPER
{
public:

    STEPPER(int, int,  int, int);
    void forwards();
    void resetStepper();

private:
    
    int revs;
    int mPin1;
    int mPin2;
    int mPin3;
    int mPin4;
    
    void reverse();
    void stepMotor(int&);
};

#endif