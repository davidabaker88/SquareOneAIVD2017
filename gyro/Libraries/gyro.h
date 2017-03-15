#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Chrono.h>
#include <LightChrono.h>

#include "tracer.h"

const float RECORD_INTERVAL = 1; // seconds
const int BUFFER_MAX = 100;

class Gyro
{
private:
    Adafruit_BNO055 m_gyro;
    
    Tracer m_history;
    sensors_event_t* m_curPoint;
	
	Chrono m_timer;
	
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