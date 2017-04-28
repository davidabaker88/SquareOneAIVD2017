#include "Gyro.h"

Gyro::Gyro(int sensorID)
{
    m_gyro = Adafruit_BNO055(sensorID);
    m_curPoint = new sensors_event_t();
}

Gyro::~Gyro()
{
}

Gyro::Direction Gyro::getDirection(float dRot)
{
    // idk if directions are right but this is a problematic solution in the first place
    if (dRot > 0) return Direction::kCW;
    else if (dRot < 0) return Direction::kCCW;
    else return Direction::kNone;
}

void Gyro::setVelocity(float x, float y, float z)
{
    m_velocity.x += x;
    m_velocity.y += y;
    m_velocity.z += z;
}

void Gyro::setDistance(float x, float y, float z)
{
    m_distance.x += x;
    m_distance.y += y;
    m_distance.z += z;
}

double deadband(double input)
{
    return (input >= -DEADBAND && input <= DEADBAND) ? 0 : input;
}

void Gyro::setup()
{
    if (!m_gyro.begin(Adafruit_BNO055::OPERATION_MODE_NDOF))
    {
        Serial.print("Gyro (BNO055) not detected");
        while (1);
    }

    uint8_t sys, g, a, m;
    while (!sys/*!m_gyro.isFullyCalibrated()*/)
    {
        m_gyro.getCalibration(&sys, &g, &a, &m);
        Serial.print(sys); Serial.print(g); Serial.print(a); Serial.println(m);
    }

    m_gyro.setExtCrystalUse(true);
    m_timer.restart();
}

void Gyro::loop()
{
    m_gyro.getEvent(m_curPoint);
    imu::Vector<3> vec = m_gyro.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    m_mag.x = vec[0]; m_mag.y = vec[1]; m_mag.z = vec[2];
    vec = m_gyro.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    m_acceleration.x = deadband(vec[0]); m_acceleration.y = deadband(vec[1]); m_acceleration.z = deadband(vec[2]);

    // Record current event if timer has passed
    if (m_timer.hasPassed(RECORD_INTERVAL))
    {
        record(m_curPoint);
        setVelocity(m_acceleration.x * RECORD_INTERVAL / 1000,
            m_acceleration.y * RECORD_INTERVAL / 1000,
            m_acceleration.z * RECORD_INTERVAL / 1000);
        setVelocity(m_distance.x * RECORD_INTERVAL / 1000,
            m_distance.y * RECORD_INTERVAL / 1000,
            m_distance.z * RECORD_INTERVAL / 1000);
        m_timer.restart();
    }
}

void Gyro::reset()
{
    m_history.clear();
    setVelocity(0, 0, 0);
    setDistance(0, 0, 0);
}

float Gyro::lastDeltaAxis(float timeFrameMs, Axis axis)
{
    int intervals = timeFrameMs / RECORD_INTERVAL;
    if (intervals >= size()) return 0;
    sensors_event_t pastEvent = m_history.get(size() - intervals);

    switch (axis)
    {
    case Axis::kXAxis:
        return m_curPoint->orientation.x - pastEvent.orientation.x;
    case Axis::kYAxis:
        return m_curPoint->orientation.y - pastEvent.orientation.y;
    case Axis::kZAxis:
        return m_curPoint->orientation.z - pastEvent.orientation.z;
    }
}

float Gyro::getOrientation(Axis axis)
{
    switch (axis)
    {
    case kXAxis:
        return m_curPoint->orientation.x;
    case kYAxis:
        return m_curPoint->orientation.y;
    case kZAxis:
        return m_curPoint->orientation.z;
    default:
        return 0;
    }
}

float Gyro::getVelocity(Axis axis)
{
    switch (axis)
    {
    case kXAxis:
        return m_velocity.x;
    case kYAxis:
        return m_velocity.y;
    case kZAxis:
        return m_velocity.z;
    default:
        return 0;
    }
}

float Gyro::getDistance(Axis axis)
{
    switch (axis)
    {
    case kXAxis:
        return m_distance.x;
    case kYAxis:
        return m_distance.y;
    case kZAxis:
        return m_distance.z;
    default:
        return 0;
    }
}

float Gyro::getDistanceFrom(float timeFrameMs, Axis axis)
{
    int intervals = timeFrameMs / RECORD_INTERVAL;
    if (intervals >= size()) return 0;
    int s = size() - intervals;
    float timeFromStart = s * RECORD_INTERVAL / 1000;
    sensors_event_t pastEvent = m_history.get(s);

    switch (axis)
    {
    case Axis::kXAxis:
        return pastEvent.acceleration.x * timeFromStart * timeFromStart;
    case Axis::kYAxis:
        return pastEvent.acceleration.y * timeFromStart * timeFromStart;
    case Axis::kZAxis:
        return pastEvent.acceleration.z * timeFromStart * timeFromStart;
    }
}

float Gyro::getHeading()
{
    return atan2(m_mag.y, m_mag.x) * (180 / 3.14159) + COORD_DECL;
}

void Gyro::record(sensors_event_t* point)
{
    if (size() >= BUFFER_MAX)
        m_history.shift();
    m_history.add(*point);
#ifdef DEBUG
    Serial.print("Gyro X: "); Serial.print(getOrientation(kXAxis));
    Serial.print("  Y: "); Serial.print(getOrientation(kYAxis));
    Serial.print("  Z: "); Serial.print(getOrientation(kZAxis));
    Serial.print("  Accel X: "); Serial.print(m_acceleration.x);
    Serial.print("  Y: "); Serial.print(m_acceleration.y);
    Serial.print("  Z: "); Serial.print(m_acceleration.z);
    Serial.print(" Heading: "); Serial.println(getHeading());
#endif // DEBUG
}