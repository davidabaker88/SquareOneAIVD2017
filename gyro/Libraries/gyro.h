#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LinkedList.h>

const float RECORD_INTERVAL = 1; // seconds

class Gyro
{
private:
    Adafruit_BNO055 m_gyro;
    
    LinkedList<sensors_event_t> m_history;
    sensor_event_t* m_curPoint;
    
public:
    Gyro(int32_t sensorID);
    ~Gyro();
    
    void setup();
    void loop();
    
    sensor_event_t* getCurrentEvent();
    
    // History
    int size();
    
};

#endif // GYRO_H