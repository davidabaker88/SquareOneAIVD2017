/* *******************************************************************
   Encoder driver function definitions by Chaoyang Liu
   ***************************************************************** */
#include "command.h"
  //below can be changed, but should be PORTD pins;
  //otherwise additional changes in the code are required
  #define FRH_ENCODER_A 38
  #define FRH_ENCODER_B 48

void fleftEncoderEvent();
void frightEncoderEvent();
void bleftEncoderEvent();
void brightEncoderEvent();

void initEncoders();
long readEncoder();
void resetEncoder();