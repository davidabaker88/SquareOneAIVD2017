#include "gyro.h"

Gyro::Gyro(int sensorID)
{
    m_gyro = Adafruit_BNO055(sensorID);
    m_curPoint = new sensors_event_t();
}

Gyro::~Gyro()
{
    
}

Direction Gyro::getDirection(float accel)
{
    // idk if directions are right but this is a problematic solution in the first place
    if (accel > 0) return Direction::kCW;
    else if (accel < 0) return Direction::kCCW;
    else return Direction::kNone; 
}

float Gyro::deltaRotate(Direction direct, float past, float current)
{
    switch (direct)
    {
    case Direction::CW:
        // unfinished
        break;
    case Direction::CCW:
        // unfinished
        break;
    default:
        return 0;
        break;
    }
}

void Gyro::setup()
{
    if (!m_gyro.begin())
	{
		Serial.print("Gyro (BNO055) not detected");
		while (1);
	}
	
	m_gyro.setExtCrystalUse(true);
	m_timer.restart();
}

void Gyro::loop()
{
	m_gyro.getEvent(m_curPoint);
	
	// Record current event if timer has passed
    if (m_timer.hasPassed(RECORD_INTERVAL))
	{
		record(m_curPoint);
		m_timer.restart();
	}
}

sensors_event_t* Gyro::getCurrentEvent()
{
    return m_curPoint;
}

float Gyro::lastDeltaAxis(float timeFrameMs, Axis axis)
{
    int intervals = timeFrameMs / RECORD_INTERVAL;
    if (intervals >= size()) return 0;
    sensors_event_t pastEvent = m_history.get(size() - intervals);
    
    switch (axis)
    {
    case Axis::kXAxis:
        return m_curPoint.orientation.x - pastEvent.orientation.x;
        break;
    case Axis::kYAxis:
        return pastEvent.orientation.y;
        break;
    case Axis::kZAxis:
        return pastEvent.orientation.z;
        break;
    }
    
}

int Gyro::size()
{
	return m_history.size();
}

void Gyro::record(sensors_event_t* point)
{
	if (size() >= BUFFER_MAX)
		m_history.shift();
	m_history.add(*point);
#ifdef DEBUG
    Serial.print("X: "); Serial.print(point->orientation.x);
    Serial.print("\tY: "); Serial.print(point->orientation.y);
    Serial.print("\tZ: "); Serial.println(point->orientation.z);
    Serial.println(size());
#endif // DEBUG
}