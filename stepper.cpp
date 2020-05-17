#include "stepper.h"



STEPPER::STEPPER(int motor1, int motor2, int motor3, int motor4)
{
    mPin1 = motor1;
    mPin2 = motor2;
    mPin3 = motor3;
    mPin4 = motor4;

    revs = 0;

    pinMode(mPin1, OUTPUT);
    pinMode(mPin2, OUTPUT);
    pinMode(mPin3, OUTPUT);
    pinMode(mPin4, OUTPUT);
}


void STEPPER::forwards()
{
  ++revs;
  
  for(int i = 0; i < 128; i++)
  {    
    for(int j= 0; j<8; j++)
    {
      stepMotor(j);
      delay(3);
    }
  }
}

void STEPPER::reverse()
{
  for(int i = 0; i < 128; i++)
  {    
    for(int j = 7; j>-1; j--)
    {
      stepMotor(j);
      delay(3);
    }
  }
}

void STEPPER::resetStepper()
{
  while(revs != 0)  
  {
    reverse();
  }
}

void STEPPER::stepMotor(int& step)
{
  switch(step)
  {
    case 0:
      digitalWrite(mPin1, LOW);
      digitalWrite(mPin2, HIGH);
      digitalWrite(mPin3, HIGH);
      digitalWrite(mPin4, HIGH);
      break;
    case 1:
      digitalWrite(mPin2, LOW);
      digitalWrite(mPin2, LOW);
      digitalWrite(mPin3, HIGH);
      digitalWrite(mPin4, HIGH);      
      break;
    case 2:
      digitalWrite(mPin1, HIGH);
      digitalWrite(mPin2, LOW);
      digitalWrite(mPin3, HIGH);
      digitalWrite(mPin4, HIGH);
      break;
    case 3:
      digitalWrite(mPin1, HIGH);
      digitalWrite(mPin2, LOW);
      digitalWrite(mPin3, LOW);
      digitalWrite(mPin4, HIGH);
      break;
    case 4:
      digitalWrite(mPin1, HIGH);
      digitalWrite(mPin2, HIGH);
      digitalWrite(mPin3, LOW);
      digitalWrite(mPin4, HIGH);
      break;
    case 5:
      digitalWrite(mPin1, HIGH);
      digitalWrite(mPin2, HIGH);
      digitalWrite(mPin3, LOW);
      digitalWrite(mPin4, LOW);
      break;
    case 6:
      digitalWrite(mPin1, HIGH);
      digitalWrite(mPin2, HIGH);
      digitalWrite(mPin3, HIGH);
      digitalWrite(mPin4, LOW);
      break;
    case 7:
      digitalWrite(mPin1, LOW);
      digitalWrite(mPin2, HIGH);
      digitalWrite(mPin3, HIGH);
      digitalWrite(mPin4, LOW);
      break;  
  }   
}