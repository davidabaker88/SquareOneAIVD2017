#include <SharpIR.h>


#ifdef Arduino
  #include "Arduino.h"
#elif defined(SPARK)
  #include "Particle.h"
  #include "math.h"
#endif
#include "SharpIR.h"

// Initialisation function
//  + irPin : is obviously the pin where the IR sensor is attached
//  + sensorModel is a int to differentiate the two sensor models this library currently supports:
//    > 1080 is the int for the GP2Y0A21Y and 
//    > 20150 is the int for GP2Y0A02YK and 
//    > 100500 is the long for GP2Y0A710K0F
//    The numbers reflect the distance range they are designed for (in cm)
SharpIR::SharpIR(int irPin, long sensorModel) {
  
    _irPin=irPin;
    _model=sensorModel;
    
    // Define pin as Input
    pinMode (_irPin, INPUT);
    
    #ifdef ARDUINO
      analogReference(DEFAULT);
    #endif
}

// Sort an array
void SharpIR::sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        bool flag = true;
        for(int o=0; o<(size-(i+1)); o++) {
            if(a[o] > a[o+1]) {
                int t = a[o];
                a[o] = a[o+1];
                a[o+1] = t;
                flag = false;
            }
        }
        if (flag) break;
    }
}

// Read distance and compute it
int SharpIR::distance() {

    int ir_val[NB_SAMPLE];
    int distanceCM;
    float current;


    for (int i=0; i<NB_SAMPLE; i++){
        // Read analog value
        ir_val[i] = analogRead(_irPin);
    }
    
    // Sort it 
    sort(ir_val,NB_SAMPLE);

    
    if (_model==1080) {
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 27.728 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.2045);
        #elif defined(SPARK)
          distanceCM = 27.728 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.2045);
        #endif

    } else if (_model==20150){

        // Previous formula used by  Dr. Marcal Casas-Cartagena
        // puntualDistance=61.573*pow(voltFromRaw/1000, -1.1068);
        
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 60.374 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.16);
        #elif defined(SPARK)
          distanceCM = 60.374 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.16);
        #endif

    } else if (_model==430){

        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        #ifdef ARDUINO
          distanceCM = 12.08 * pow(map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.058);
        #elif defined(SPARK)
          distanceCM = 12.08 * pow(map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000)/1000.0, -1.058);
        #endif
        
    } else if (_model==100500){
        
        #ifdef ARDUINO
          current = map(ir_val[NB_SAMPLE / 2], 0, 1023, 0, 5000);
        #elif defined(SPARK)
          current = map(ir_val[NB_SAMPLE / 2], 0, 4095, 0, 5000);
        #endif
        // use the inverse number of distance like in the datasheet (1/L)
        // y = mx + b = 137500*x + 1125 
        // x = (y - 1125) / 137500
        // Different expressions required as the Photon has 12 bit ADCs vs 10 bit for Arduinos
        if (current < 1400 || current > 3300) {
          //false data
          distanceCM = 0;
        } else {
          distanceCM = 1.0 / (((current - 1125.0) / 1000.0) / 137.5);
        }
    }

    return distanceCM;
}