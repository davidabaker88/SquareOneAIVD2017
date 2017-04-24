#include <Arduino.h>
#include <PID_v1.h>

//Start Steering Includes
#include <Servo.h>
//End Steering Includes

//Start Drive Includes
//THIS IS A LOCALLY DEFINED LIBRARY YOU IMPORT WITH THIS ARDUINO SCRIPT ON GITHUB NEEDED FOR (V)ESC CONTROL
#include <VescUart.h>
//End Drive Includes

//Start 9DoF Includes
#include <gyro.h>
//End 9DoF Includes

//Start GPS Includes
//End GPS Includes

//Start Magnetic Includes
//End Magnetic Includes

//Start shared Includes
//End shared Includes

//All Pins should be a define or const int.  And should be in All caps

//Start Steering Defines and Global Variables
const int STEERING_PIN = 9;
const int STEERING_P = 1.0;
const int STEERING_I = 0;
const int STEERING_D = 0;

Servo myServo;

void steering(float degreeIn);
//End Steering Defines and Global Variables

//Start Drive Defines and Global Variables
//Call this function and give it a float to set the motor current at
//Max RPM For testing currently hard coded in the VESC software is 2000 RPM
//The motor will try to always get to 2000 RPM with a current you give it
//You need more current to let the motor take more load to get it to higher speeds
//The hardcoded max current is 10.0 MAX and -10.0 MIN (setting negative current values
//will make the car go backwards.
void setSpeed(float Current);
void setBrake();
//End Drive Defines and Global Variables

//Start 9DoF Defines and Global Variables
const int GYRO_SENSOR_ID = 55;

Gyro gyro{ GYRO_SENSOR_ID };
//End 9DoF Defines and Global Variables

//Start GPS Defines and Global Variables
//End GPS Defines and Global Variables

//Start Magnetic Defines and Global Variables
//End Magnetic Defines and Global Variables

//Start Sensor Arduino Setup
const int SONIC_FRONT_PIN = 0;
const int SONIC_RIGHT_PIN = 1;
const int SONIC_BACK_PIN = 2;
const int SONIC_LEFT_PIN = 3;
const int SONIC_FRONT_RIGHT_PIN = 4;
const int SONIC_BACK_RIGHT_PIN = 5;
const int SONIC_BACK_LEFT_PIN = 6;
const int SONIC_FRONT_LEFT_PIN = 7;

//End Sensor Arduino Setup

//Start shared Globals and 
enum TASK {
    none,//do nothing task
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight
};

TASK currentTask = none;
//End shared Globals and Defines

void setup() {
    //Start Initilization
    Serial.begin(115200);
    //End Initialization
    //Start Steering Setup
    Serial.begin(9600);
    myServo.attach(STEERING_PIN);
    //End Steering Setup
    //Start Drive Setup
    Serial1.begin(115200);
    //End Drive Setup
    //Start 9DoF Setup
    //End 9DoF Setup
    //Start GPS Setup
    //End GPS Setup
    //Start Magnetic Setup
    //End Magnetic Setup
    //Start Sensor Arduino Setup
    //End Sensor Arduino Setup

}

void loop() {
    if (currentTask == none) {
        setBrake();
    }
    if (currentTask == one)
    {
        //start task 1 Code:  go forward X meters turn right.
        //end task1 Code
    }
    if (currentTask == two)
    {
        //start task 2 Code:  go forward X meters if obstacle, stop.
        //end task2 Code
    }
    if (currentTask == three)
    {
        //start task 3 Code:  go forward X meters turn right.
        //end task3 Code
    }
    if (currentTask == four)
    {
        //start task 4 Code:  navagate fixed course.
        //end task4 Code
    }
    if (currentTask == five)
    {
        //start task 5 Code:  avoid obstacles/relatively straight.
        //end task5 Code
    }
    if (currentTask == six)
    {
        //start task 6 Code:  Parallel park.
        //end task6 Code
    }
    if (currentTask == seven)
    {
        //start task 7 Code:  traffic light.
        //end task7 Code
    }
    if (currentTask == eight)
    {
        //start task 8 Code:  gps navagation.
        //end task8 Code
    }
}

void setSpeed(float Current) {
    VescUartSetCurrent(Current);
}

void setBrake() {
    VescUartSetCurrentBrake(0.0);
}

void steering(float degreeIn) {
  float degree = degreeIn;
  degree += 90;
  if (degree > 180) {
      degree = 180;
  }
  myServo.write(degree);
  Serial.write((int)degree);
}
