#include "string.h"
#include "ctype.h"
#include "SoftwareSerial.h"
#include "dGPS.h"

// Software serial TX & RX Pins for the GPS module
// Initiate the software serial connection

int ledPin = 13;                  // LED test pin
float desLat=0;                   //Destination Latitude filled by user in Serial Monitor Box
float desLon=0;                   //Destination Longitude filled by user in Serial Monitor Box
float destAngle;
char fla[2];                      //flag (Y/N) whether to print checksum or not. Filled by user in Serial Monitor Box
dGPS dgps = dGPS();               // Construct dGPS class


float getdestcoord()
  // function to get the coordinates of the destination from user
  { float result;
    while (Serial.available()==0)
    {;}// do nothing until something comes into the serial buffer

    if (Serial.available()>0)
    {
     result=Serial.parseFloat(); //read a float value from serial monitor box, correct upto 2 decimal places
     delay(10); // the processor needs a moment to process
    }
    return result;
  }
  
 void getflag(char *str)
   // function to read the flag character from the user 
  {  
    while (Serial.available()==0)
    {;}
    int index=0;
    if (Serial.available()>0)
    { if (index<2){
      str[index]=Serial.read();
      index++;
     }
      else str[index]='\0';
      delay(10);
    }  
  }

void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.end();                  // Close any previously established connections
  Serial.begin(9600);            // Serial output back to computer.  On.
  dgps.init();                   // Run initialization routine for dGPS.
  delay(1000);  
  Serial.print("Enter Destination Latitude (in degrees): ");
  delay(1000);
  desLat=getdestcoord();
  Serial.println(desLat);
  
  Serial.print("Enter Destination Longitude (in degrees): ");
  delay(3000);
  desLon=getdestcoord();
  Serial.println(desLon);  
  
}

void loop() {
  
  dgps.update(desLat, desLon);    // Calling this updates the GPS data.  The data in dGPS variables stays the same unless
                                  // this function is called.  When this function is called, the data is updated.
  
  Serial.println("Latitude: ");
  Serial.print(dgps.Lat(), 6);    // Lattitude - in DD.MMSSSS format (decimal-degrees format)  (D-Degree; M-Minute; S-Second)
  Serial.println(" degrees");
  
  Serial.print("Longitude: ");
  Serial.print(dgps.Lon(), 6);    // Longitude - in DD.MMSSSS format (decimal-degrees format)  (D-Degree; M-Minute; S-Second)
  Serial.println(" degrees");
  
  Serial.print("Slope: ");
  Serial.print(dgps.angleToPoint());
  Serial.println(" degrees");


  
  
  

  

}
