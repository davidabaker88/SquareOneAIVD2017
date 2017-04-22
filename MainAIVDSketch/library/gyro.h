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

// Summary:
// Wrapper/Manager for the Adafruit BNO055 9DoF sensor
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
    sensors_vec_t m_velocity;
    sensors_vec_t m_distance;

    Chrono m_timer;

    // Find the current direction of the gyro is rotating from the change in rotation
    // Takes: dRot - change in rotation
    Direction getDirection(float dRot);

    // Sets the Velocity/Distance vectors
    void setVelocity(float x, float y, float z);
    void setDistance(float x, float y, float z);

public:
    // Constructs with sensorID which is 55
    Gyro(int sensorID);
    ~Gyro();

    // Methods to be called in the respective functions in the main arduino ino
    void setup();
    void loop();

    // Get this iterations sensor event
    sensors_event_t* getCurrentEvent() { return m_curPoint; }

    // Get the change in an axis in the time frame
    // Takes: timeFrameMs - time from now in ms to compare with now
    //        axis - axis to use values from
    float lastDeltaAxis(float timeFrameMs, Axis axis);

    // Get the velocity/distance from a certain axis
    float getVelocity(Axis axis);
    float getDistance(Axis axis);

    // Get the distance in meters from an axis in a time frame
    // Takes: timeFrameMs - time from now in ms to compare with now
    //        axis - axis to use values from
    // WARNING: This calculation's distance is relative to the list
    //      so it forgets distances from points that were deleted
    float getDistanceFrom(float timeFrameMs, Axis axis);

    // Gets the heading in degrees from true north
    // Takes: lat - current latitudinal coordinate
    //        lon - current longitudinal coordinate
    float getHeading(float lat, float lon);

    // History
    // Gets the current size of the history list
    int size() { return m_history.size(); }

    // Record a point in the list
    void record(sensors_event_t* point);
};

// EXAMPLES -------------------------------------------------------------------
// Change in rotation:
//  Gyro gyro = Gyro(55);
//  float timeFrame = 500; // ms
//  Gyro::Axis axis = Gyro::kXAxis;
//
//  void setup() {
//      Serial.begin(9600);
//      gyro.setup();
//  }
//
//  void loop() {
//      gyro.loop();
//
//      Serial.print("dRotation: ");
//      // This will return 0 until 500 ms has passed
//      Serial.println(gyro.lastDeltaAxis(timeFrame, axis));
//  }
//
// Distance from a time period:
//  Gyro gyro = Gyro(55);
//  float timeFrame = 500; // ms
//  Gyro::Axis axis = Gyro::kXAxis;
//
//  void setup() {
//      Serial.begin(9600);
//
//      gyro.setup();
//  }
//
//  void loop() {
//      gyro.loop();
//
//      float dist = gyro.getDistanceFrom(timeFrame, axis);
//      Serial.print("Distance change since 500 ms ago: "); Serial.println(dist);
//  }
//

#endif // GYRO_H