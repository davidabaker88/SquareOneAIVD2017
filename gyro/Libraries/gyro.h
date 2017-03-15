#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LightChrono.h>

#include "tracer.h"

#define DEBUG

const float RECORD_INTERVAL = 1000; // ms
const int BUFFER_MAX = 100;

class Gyro
{
private:
    Adafruit_BNO055 m_gyro;
    
    Tracer m_history;
    sensors_event_t* m_curPoint;
	
	LightChrono m_timer;
	
public:
    Gyro(int sensorID);
    ~Gyro();
    
    void setup();
    void loop();
    
    sensors_event_t* getCurrentEvent();
    
    // History
    int size();
    void record(sensors_event_t* point);
};

#endif // GYRO_H