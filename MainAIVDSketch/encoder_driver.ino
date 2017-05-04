/*
Name:    encoder_driver.h
Created:  7/25/2016 9:36:52 PM
Author: Chaoyang Liu
*/

#include "encoder_driver.h"

// variables to store the number of encoder pulses
// for each motor
volatile unsigned long encoderCount = 0;

    pinMode(FLH_ENCODER_A, INPUT);
    pinMode(FLH_ENCODER_B, INPUT);
    
    // initialize hardware interrupts
    attachInterrupt(2, fleftEncoderEvent, CHANGE);


    Serial.begin(57600);
  }

  // encoder event for the interrupt call
  void fleftEncoderEvent() {
    if (digitalRead(FLH_ENCODER_A) == HIGH) {
      if (digitalRead(FLH_ENCODER_B) == LOW) {
        fleftCount++;
      }
      else {
        //fleftCount--;
      }
    }
    else {
      if (digitalRead(FLH_ENCODER_B) == LOW) {
        fleftCount--;
      }
      else {
        //fleftCount++;
      }
    }
  }



long readEncoder() {
      return encoderCount;
}

  /* Wrap the encoder reset function */
  void resetEncoder() {
    
      encoderCount = 0L;
    
  }


