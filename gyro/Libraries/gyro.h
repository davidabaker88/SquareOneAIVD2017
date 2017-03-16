#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Chrono.h>

#include "tracer.h"

//#define DEBUG

const float RECORD_INTERVAL = 1000; // ms
const int BUFFER_MAX = 100;

class Gyro
{
private:
    Adafruit_BNO055 m_gyro;
    
    Tracer m_history;
    sensors_event_t* m_curPoint;
	
	Chrono m_timer;
    
    Direction getDirection(sensors_event_t* event);
    float deltaRotate(Direction direct, float past, float current);
	
public:
    enum Axis
    {
        kXAxis,
        kYAxis,
        kZAxis
    };
    
    enum Direction
    {
        kCW,
        kCCW,
        kNone
    }

    Gyro(int sensorID);
    ~Gyro();
    
    void setup();
    void loop();
    
    sensors_event_t* getCurrentEvent();
    float lastDeltaAxis(float timeFrameMs, Axis axis);
    
    // History
    int size();
    void record(sensors_event_t* point);
};

#endif // GYRO_H