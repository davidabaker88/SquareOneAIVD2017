#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Chrono.h>
#include <math.h>

#include "tracer.h"

#define DEBUG

const float RECORD_INTERVAL = 500; // ms
const int BUFFER_MAX = 100;

const float COORD_LAT = 42.300688;
const float COORD_LONG = 83.698129;
const float COORD_DECL = 6.53; // +- .37 // 7.04 Mcity

const float DEADBAND = 0.05;

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
    sensors_vec_t m_mag;
    sensors_vec_t m_acceleration;
    sensors_vec_t m_velocity;
    sensors_vec_t m_distance;

    Chrono m_timer;

    // Find the current direction of the gyro is rotating from the change in rotation
    // Takes: dRot - change in rotation
    Direction getDirection(float dRot);

    // Sets the Velocity/Distance vectors
    void setVelocity(float x, float y, float z);
    void setDistance(float x, float y, float z);

    double deadband(double input);

public:
    // Constructs with sensorID which is 55
    Gyro(int sensorID);
    ~Gyro();

    // Methods to be called in the respective functions in the main arduino ino
    void setup();
    void loop();

    void reset();

    // Get this iterations sensor event
    sensors_event_t* getCurrentEvent() { return m_curPoint; }

    // Get the change in an axis in the time frame
    // Takes: timeFrameMs - time from now in ms to compare with now
    //        axis - axis to use values from
    float lastDeltaAxis(float timeFrameMs, Axis axis);

    // Get the orientation/velocity/distance from a certain axis
    float getOrientation(Axis axis);
    float getVelocity(Axis axis);
    float getDistance(Axis axis);

    // Get the distance in meters from an axis in a time frame
    // Takes: timeFrameMs - time from now in ms to compare with now
    //        axis - axis to use values from
    // WARNING: This calculation's distance is relative to the list
    //      so it forgets distances from points that were deleted
    float getDistanceFrom(float timeFrameMs, Axis axis);

    // Gets the heading in degrees from true north
    float getHeading();

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
// Controlling direction with PID
//  Servo servo;
//  Gyro gyro{ 55 };
//  double in, out, sp;
//  // PID values need to be tuned
//  PID pid{ &in, &out, &sp, 1, 0, 0, DIRECT };
//
//  void steering(float degreeIn);
//
//  void setup() {
//      gyro.setup();
//      servo.attach(9);
//
//      // Maintain 0 degree heading
//      sp = 90;
//  }
//
//  void loop() {
//      gyro.loop();
//
//      // Set 'in' to what we currently are at
//      in = gyro.getOrientation(Gyro::kXAxis);
//      pid.Compute();
//      // Send PID output to steering
//      steering(out);
//  }
//
//  void steering(float degreeIn) {
//      float degree = degreeIn;
//      degree += 90;
//      if (degree > 180) {
//          degree = 180;
//      }
//      myServo.write(degree);
//      Serial.write((int)degree);
//  }

#endif // GYRO_H