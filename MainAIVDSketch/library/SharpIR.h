/*
 * SharpIR
 * Library for the acquisition of distance data from Sharp IR sensors
 *
 * Created by Giuseppe Masino, 28 may 2016
 * Author URL http://www.facebook.com/dev.hackerinside
 * Author email: dev.giuseppemasino@outlook.it
 *
 * This library and the relative example files are released under the
 * CreativeCommons ShareAlike-Attribution 4.0 International license 
 * and a custom MIT license.
 *
 * The license terms can be read at the following url: https://www.facebook.com/notes/giuseppe-masino/faqs-about-my-works/197854657355058
 *
 */

#pragma once

#include <Arduino.h>
/*
	SharpIR
	Arduino library for retrieving distance (in cm) from the analog GP2Y0A21Y and GP2Y0A02YK
	From an original version of Dr. Marcal Casas-Cartagena (marcal.casas@gmail.com)     
	
    Version : 1.0 : Guillaume Rico
	https://github.com/guillaume-rico/SharpIR
*/

#ifndef SharpIR_h
#define SharpIR_h

#define NB_SAMPLE 10

#ifdef ARDUINO
  #include "Arduino.h"
#elif defined(SPARK)
  #include "Particle.h"
#endif

class SharpIR
{
  public:

    SharpIR (int irPin, long sensorModel);
    int distance();

  private:

    void sort(int a[], int size);
    
    int _irPin;
    long _model;
};

#endif