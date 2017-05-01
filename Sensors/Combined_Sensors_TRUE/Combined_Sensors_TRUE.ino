#include <NewPing.h>

#include <SharpIR.h>
#include "math.h"
#include "NewPing.h"
#define FRONT_THRESHOLD 80
#define QUADRANT 1
#define MAX_DISTANCE 500
#define Q_PIN_0 23
#define SONAR_NUM     2
int IR_PIN1 = 1;
int IR_PIN2 = 2;
int IR_PIN3 = 3;
int IR_PIN4 = 4;
int IR_PIN5 = 5;
int IR_PIN6 = 6;
int IR_PIN7 = 7;
int IR_PIN8 = 8;


//Precision = Threshold / ((2^numbits)-1); IE: Precesion = 50 cm when numbits = 2 & Threshold = 150

int outPinArray1[] = {30,31,32};//Left wheel
int outPinArray2[] = {33,34,35,36};//Front left 
int outPinArray3[] = {37,38,39,40};//Front Right
int outPinArray4[] = {41,42,43};//Right wheel
int outPinArray5[] = {44};//back right side
int outPinArray6[] = {45,46,47,48};// back right
int outPinArray7[] = {49,50,51,52};//back left
int outPinArray8[] = {53};//back left side

int WheelTrashhold = 150;
int FrontTrashhold = 150;
int BackTrashhold = 150;
int BacksideTrashhold = 150;

unsigned int cm[SONAR_NUM];

byte disToShow1;
boolean Quadrant[1];
int IR_PRECISION = 94;
int IR_MODEL = 20150;
SharpIR irSense1(IR_PIN1, IR_MODEL);
SharpIR irSense2(IR_PIN2, IR_MODEL);
SharpIR irSense3(IR_PIN3, IR_MODEL);
SharpIR irSense4(IR_PIN4, IR_MODEL);
SharpIR irSense5(IR_PIN5, IR_MODEL);
SharpIR irSense6(IR_PIN6, IR_MODEL);
SharpIR irSense7(IR_PIN7, IR_MODEL);
SharpIR irSense8(IR_PIN8, IR_MODEL);

  NewPing sonarSensorA(53, 52, MAX_DISTANCE);
  NewPing sonarSensorB(51, 50, MAX_DISTANCE); // (trigger, echo)
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200) ;

  for(int i = 30; i<53; i++)
  {
    pinMode(i, OUTPUT);

   
  }
   //sonic
    pinMode(Q_PIN_0, OUTPUT);
    for (uint8_t i = 0; i < QUADRANT; i++){
    Quadrant[i] = false;
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int dis = 0;
  dis= irSense1.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray1[0]),dis,outPinArray1,WheelTrashhold);
  
  int dis2 = irSense2.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray2[0]),dis,outPinArray2,FrontTrashhold);
  
  int dis3 = irSense3.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray3[0]),dis,outPinArray3,FrontTrashhold);
  
  int dis4 = irSense4.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray4[0]),dis,outPinArray4,WheelTrashhold);
  
  int dis5 = irSense5.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray5[0]),dis,outPinArray5,BacksideTrashhold);
  
  int dis6 = irSense6.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray6[0]),dis,outPinArray6,BackTrashhold);
  
  int dis7 = irSense7.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray7[0]),dis,outPinArray7,BackTrashhold);
  
  int dis8 = irSense8.getDistance();
  BinaryConverterAndOutput(sizeof(outPinArray8[0]),dis,outPinArray8,BacksideTrashhold);

  //sonic sensors
  cm[0] =sonarSensorA.ping_median(10 ,MAX_DISTANCE);
  cm[1] =sonarSensorB.ping_median(10 ,MAX_DISTANCE);
  Serial.println(cm[0]);
 if  ((cm[0] < FRONT_THRESHOLD || cm[1] < FRONT_THRESHOLD) && (cm[0] > 0 || cm[1] > 0))
  {
    Quadrant[0] = true;
  }
  else {
    Quadrant[0] = false;
  }
}


void BinaryConverterAndOutput(int numBits, int distanceConv,int outPinArray[],int THRESHOLD)
{
    if(distanceConv > THRESHOLD){
      distanceConv = 0; 
    }
    int stepVariable = THRESHOLD/ (pow(2.0,(double)numBits)-1);
    double outTemp = (double)distanceConv / stepVariable;
    distanceConv = ceil(outTemp);
    for (int i = 0; i < numBits; i++)
    {
       if (bitRead(distanceConv, i) == 1)
      {
        digitalWrite(outPinArray[i], HIGH);
      }
      else
      {
        digitalWrite(outPinArray[i], LOW);
      }
    }
}

