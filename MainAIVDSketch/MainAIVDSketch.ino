#include <Arduino.h>
#include <PID_v1.h>
#include <math.h>

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
#include <dGPS.h>
//End GPS Includes

//Start Magnetic Includes
//End Magnetic Includes

//Start shared Includes
//End shared Includes

//All Pins should be a define or const int.  And should be in All caps

//Start Steering Defines and Global Variables
const int STEERING_PIN = 9;
const double STEERING_P = 1.0;
const double STEERING_I = 0;
const double STEERING_D = 0;

double in, out, sp;
PID pid{ &in, &out, &sp, STEERING_P, STEERING_I, STEERING_D, DIRECT };

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
dGPS gps;
//End GPS Defines and Global Variables

//Start Magnetic Defines and Global Variables
//End Magnetic Defines and Global Variables

//Start Sensor Arduino Setup
const int SONIC_FRONT_PIN = 0;

const int IR_A_PILLAR_LEFT_PIN[] = { 0 };
const int IR_FRONT_LEFT_PIN[] = { 0 };
const int IR_FRONT_RIGHT_PIN[] = { 0 };
const int IR_A_PILLAR_RIGHT_PIN[] = { 0 };
const int IR_CORNER_RIGHT_PIN[] = { 0 };
const int IR_BACK_RIGHT_PIN[] = { 0 };
const int IR_BACK_LEFT_PIN[] = { 0 };
const int IR_CORNER_LEFT_PIN[] = { 0 };

int IRDistance(int pins[], int length = -1);
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

//Task variables
Chrono time;
int preTurnSp;

//task 1
int t1Stage;
//task 2
int t2Stage;
//task 3
//task 4
int t4Stage;
//task 5
int t5Stage;
//task 5
//task 6
//task 7
const float lat[] = { 0, 0 };
const float lon[] = { 0, 0 };

int t7Stage;
int destStage;
//task 8
//End task variables

//End shared Globals and Defines

void setup() {
    //Start Initilization
    Serial.begin(115200);
    //End Initialization
    //Start Steering Setup
    myServo.attach(STEERING_PIN);
    //End Steering Setup
    //Start Drive Setup
    Serial1.begin(115200);
    //End Drive Setup
    //Start 9DoF Setup
    gyro.setup();
    //End 9DoF Setup
    //Start GPS Setup
	gps.init();
    //End GPS Setup
    //Start Magnetic Setup
    //End Magnetic Setup
    //Start Sensor Arduino Setup
    //End Sensor Arduino Setup
}


void loop() {
	
    gyro.loop();
	in = gyro.getOrientation(Gyro::kXAxis);
    pid.Compute();
    steering(out);

    if (currentTask == none) {
        //setBrake();
    }
    if (currentTask == one)
    {
        //start task 1 Code:  go forward X meters turn right.
        switch (t1Stage)
        {
        case 0:
            time.restart();
            t1Stage++;
            break;
        case 1:
            if (time.hasPassed(5000))
            {
                setSpeed(5.0);
                time.restart();
                t1Stage++;
            }
            break;
        case 2:
            if (gyro.getDistanceFrom(time.elapsed(), Gyro::kXAxis) >= 3)
            {
                time.restart();
                sp = 90;
                t1Stage++;
            break;
            }
        case 3:
            if (in >= 89 && in <= 91)
            {
                time.restart();
                t1Stage++;
            }
            break;
        case 4:
            if (gyro.getDistanceFrom(time.elapsed(), Gyro::kXAxis) >= 3)
            {
                setSpeed(0);
				t1Stage++;

                //done
            }
        }
        //end task1 Code
    }
    if (currentTask == two)
    {
        //start task 2 Code:  go forward X meters if obstacle, stop.
		switch (t2Stage)
		{
		case 0:
			time.restart();
			setSpeed(5.0);
			t2Stage++;
			break;
		case 1:
			if (!!digitalRead(SONIC_FRONT_PIN) || !!IRDistance(IR_FRONT_LEFT_PIN) || !!IRDistance(IR_FRONT_RIGHT_PIN))
			{
				setSpeed(0);
			}
			else if (gyro.getDistanceFrom(time.elapsed(), Gyro::kXAxis) >= 11)
			{
				setSpeed(0);
				t2Stage++;

				//done
			}
			else 
			{
				setSpeed(5);
			}
			break;
		}
        //end task2 Code
    }
    if (currentTask == three)
    {
        //start task 3 Code:  navagate fixed course.
        //end task3 Code
    }
    if (currentTask == four)
    {
        //start task 4 Code:  avoid obstacles/relatively straight.

		setSpeed(5);

		switch (t4Stage)
		{
		case 0:
			preTurnSp = sp;

			if (!!IRDistance(IR_FRONT_LEFT_PIN))
			{
				sp += 90;
				t4Stage++;
			}
			else if (!!IRDistance(IR_FRONT_RIGHT_PIN) || !!digitalRead(SONIC_FRONT_PIN))
			{
				sp -= 90;
				t4Stage += 2;
			}
			break;

		case 1: // turned right
		{
			if (in >= fmod(preTurnSp + 89, 360) && in <= fmod(preTurnSp + 91, 360))
			{
				if (!(!!IRDistance(IR_A_PILLAR_LEFT_PIN)))
				{
					sp = preTurnSp;
					t4Stage = 3;
				}
			}

			break;
		}

		case 2: //turned left
		{
			int low = preTurnSp - 91;
			if (low < 0) low += 360;
			int high = fmod(low + 2, 360);

			if (in >= low && in <= high)
			{
				if (!(!!IRDistance(IR_A_PILLAR_RIGHT_PIN)))
				{
					sp = preTurnSp;
					t4Stage = 3;
				}
			}

			break;
		}

		case 3: //turned back forward
		{

			int low = sp - 1;
			if (low < 0) low += 360;
			int high = fmod(sp + 1, 360);

			if (in >= low && in <= high)
				t4Stage = 0;

			break;
		}
		}

        //end task4 Code
    }
    if (currentTask == five)
    {
        //start task 5 Code:  Parallel park.
        switch (t5Stage)
        {
            case 0: // Drive forward looking for a spot large enough
                steering(0);
            break;
            case 1: // Turn 45 and back up; continue if at a-pillar
                steering(45);
            break;
            case 2: // Turn -45 and continue backing up; may have to see if it hits other car 
                steering(-45);
            break;
            case 3: // Turn 0 and back up to an appropriate distance
                steering(0);
            break;
        }
        //end task5 Code
    }
    if (currentTask == six)
    {
        //start task 6 Code:  traffic light.
        //end task6 Code
    }
    if (currentTask == seven)
    {
        //start task 7 Code:  gps navagation.
		gps.update(lat[destStage], lon[destStage]);

		switch (t7Stage)
		{
		case 0: // navigating
			//assuming pointing north
			setSpeed(5);
			sp = gps.angleToPoint();

			if (gps.Dist() <= 0.5)
			{
				setSpeed(0);
				time.restart();
				t7Stage++;
			}
			//Turning
			else if (!!IRDistance(IR_FRONT_LEFT_PIN))
			{
				preTurnSp = sp;
				sp += 90;
				t7Stage = 2;
			}
			else if (!!IRDistance(IR_FRONT_RIGHT_PIN) || !!digitalRead(SONIC_FRONT_PIN))
			{
				preTurnSp = sp;
				sp -= 90;
				t7Stage = 3;
			}

			break;
		case 1: 
			
			// blinking light or something

			if (time.elapsed() < 10000) // wait 10 seconds
				if (destStage + 1 < sizeof(lat) / sizeof(lat[0]) && destStage + 1 < sizeof(lon) / sizeof(lon[0])) // size of array
				{
					t7Stage = 0;
					destStage++;
				}
				else
				{
					t7Stage++;
					destStage = 0;
				}

			break;

		case 2: //right turn
			if (in >= fmod(preTurnSp + 89, 360) && in <= fmod(preTurnSp + 91, 360))
			{
				if (!(!!IRDistance(IR_A_PILLAR_LEFT_PIN)))
				{
					sp = preTurnSp;
					t4Stage = 4;
				}
			}

			break;

		case 3: //left turn
		{
			int low = preTurnSp - 91;
			if (low < 0) low += 360;
			int high = fmod(low + 2, 360);

			if (in >= low && in <= high)
			{
				if (!(!!IRDistance(IR_A_PILLAR_RIGHT_PIN)))
				{
					sp = preTurnSp;
					t7Stage = 4;
				}
			}

			break;
		}

		case 4: 
		{

			int low = sp - 1;
			if (low < 0) low += 360;
			int high = fmod(sp + 1, 360);

			if (in >= low && in <= high)
				t4Stage = 0;

			break;
		}
		}

        //end task7 Code
    }
    if (currentTask == eight)
    {
        //start task 8 Code:  Platoon?
        //end task8 Code
    }

	if (sp < 0)
		sp += 360;

	sp = fmod(sp, 360);
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
  //Serial.write((int)degree);
}

//Takes IR pins from least to most and converts into distance (cm)
int IRDistance(const int pins[], int length = -1)
{
	if (length == -1)
		length = sizeof(pins) / sizeof(pins[0]);

	int num;
	for (int i = 0; i < length; i++)
		if (digitalRead(pins[i]) == 1)
			num += pow(2, i);

	return (int)(num * (150.0 / pow(2, length)));
}
