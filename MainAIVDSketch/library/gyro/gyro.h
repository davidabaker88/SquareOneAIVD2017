#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Chrono.h>
#include <math.h>

#include "tracer.h"

#define DEBUG

const float RECORD_INTERVAL = 1000; // ms
const int BUFFER_MAX = 100;

const float COORD_LAT = 42.300688;
const float COORD_LONG = 83.698129;
const float COORD_DECL = 6.53; // +- .37 // 7.04 Mcity


class Gyro
{
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
    };
    
private:
    Adafruit_BNO055 m_gyro;
    
    Tracer m_history;
    sensors_event_t* m_curPoint;
	
	Chrono m_timer;
    
    Direction getDirection(float accel)
    {
        // idk if directions are right but this is a problematic solution in the first place
        if (accel > 0) return Direction::kCW;
        else if (accel < 0) return Direction::kCCW;
        else return Direction::kNone; 
    }
    
    float deltaRotate(Direction direct, float past, float current)
    {
        switch (direct)
        {
        case Direction::kCW:
            // unfinished
            break;
        case Direction::kCCW:
            // unfinished
            break;
        default:
            return 0;
            break;
        }
    }
	
public:
    Gyro(int sensorID)
    {
        m_gyro = Adafruit_BNO055(sensorID);
        m_curPoint = new sensors_event_t();
    }
    
    ~Gyro()
    {
    }
    
    void setup()
    {
        if (!m_gyro.begin())
        {
            Serial.print("Gyro (BNO055) not detected");
            while (1);
        }
        
        m_gyro.setExtCrystalUse(true);
        m_timer.restart();
    }
    
    void loop()
    {
        m_gyro.getEvent(m_curPoint);
        
        // Record current event if timer has passed
        if (m_timer.hasPassed(RECORD_INTERVAL))
        {
            record(m_curPoint);
            m_timer.restart();
        }
    }
    
    sensors_event_t* getCurrentEvent() { return m_curPoint; }
    
    float lastDeltaAxis(float timeFrameMs, Axis axis)
    {
        int intervals = timeFrameMs / RECORD_INTERVAL;
        if (intervals >= size()) return 0;
        sensors_event_t pastEvent = m_history.get(size() - intervals);
        
        switch (axis)
        {
        case Axis::kXAxis:
            return m_curPoint->orientation.x - pastEvent.orientation.x;
            break;
        case Axis::kYAxis:
            return pastEvent.orientation.y;
            break;
        case Axis::kZAxis:
            return pastEvent.orientation.z;
            break;
        }
    }
    
    float getHeading(float lat, float lon)
    {
        return atan2(m_curPoint->magnetic.y, m_curPoint->magnetic.x) * (180 / 3.14159) + COORD_DECL; // STUPID and probably doesn't work
    }
    
    // History
    int size() { return m_history.size(); }
    
    void record(sensors_event_t* point)
    {
        if (size() >= BUFFER_MAX)
            m_history.shift();
        m_history.add(*point);
#ifdef DEBUG
        Serial.print("X: "); Serial.print(point->orientation.x);
        Serial.print("\tY: "); Serial.print(point->orientation.y);
        Serial.print("\tZ: "); Serial.println(point->orientation.z);
#endif // DEBUG
    }
};

#endif // GYRO_H