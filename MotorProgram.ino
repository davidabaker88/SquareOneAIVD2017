                                                                                                                                             
//  NAME         : JIM BUI
//  NAME         : SERGEI DINES
//  NAME         : Dominic Cox
//  Affilation     : M.E.C.H. - University of South Florida
//  PURPOSE        : Motor + Servo Control Xbox 360 Controller
//  Revised        : 4 / 24 / 2015

#include <XBOXRECV.h>

USB Usb ;  
XBOXRECV Xbox(&Usb) ;

int motorDir = 8 ; // Dir
int motorSpeed = 9; // PWM

int motorDir2 = 4; // Dir
int motorSpeed2 = 3; // PWM

int FWD1 = 1;
int BW1 = 0;
int FWD2 = 0;
int BW2 = 1;

void setup()
{
	Serial.begin(115200) ;
	Usb.Init() ; 
	Serial.println( "Begin speed log." ) ;
	pinMode(motorDir , OUTPUT) ;
        pinMode(motorDir2, OUTPUT);
        pinMode(motorSpeed, OUTPUT);
        pinMode(motorSpeed2, OUTPUT);
}
void loop() 
{

	Usb.Task() ; 
	if (Xbox.XboxReceiverConnected) 
	{
		int speed ; 
    if (Xbox.getAnalogHat(LeftHatY , 0) > 10000)
		{
			  speed = Xbox.getAnalogHat(LeftHatY , 0) / 131 ;
       //code for when left joystick is up
        //digitalWrite(motorDir,1);
       // digitalWrite(motorDir2, 0);
			 // analogWrite(motorSpeed , speed) ;
       // analogWrite(motorSpeed2, speed);
        //debug info  for speed
			  Serial.print( "Forward: " ); 
			  //Serial.print(Xbox.getAnalogHat(LeftHatY , 0) / 131) ;
			  Serial.println() ;
		}
    else if(Xbox.getAnalogHat(LeftHatY, 0) < -10000)
    {
      //code to go backwards
        speed = -Xbox.getAnalogHat(LeftHatY , 0) / 131 ;
        //digitalWrite(motorDir, 0);
       // digitalWrite(motorDir2, 1);
			 // analogWrite(motorSpeed , speed) ;
       // analogWrite(motorSpeed2, speed);
        //debug info
			  Serial.print( "Backwards: " ) ;
			  //Serial.print(Xbox.getAnalogHat(LeftHatY , 0) / 131) ;
			  Serial.println() ;
		} 
		else
		{
       Serial.print( "Nothing: " ) ;
        //Serial.print(Xbox.getAnalogHat(LeftHatY , 0) / 131) ;
        Serial.println() ;
      //should be zero aka not moving
			 // analogWrite(motorSpeed , 0) ;
       // analogWrite(motorSpeed2, 0);
		}
   if(Xbox.getAnalogHat(RightHatX , 0) < -10000)
   {
    //code for left joystick
    Serial.print("Left: ");
    
   }
   else if (Xbox.getAnalogHat(RightHatX , 0) > 10000)
   {
    //code for left joystick
    Serial.print("Right: ");
    
   }
   if(Xbox.getButtonPress(X, 0))
   {
    Serial.print("ButtonPressed");
   }
   
		delay(1) ;
	}
delay(1) ;
}
      
      
      


