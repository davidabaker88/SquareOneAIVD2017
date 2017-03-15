#include "gyro.h"

Gyro::Gyro(int sensorID)
{
    m_gyro = Adafruit_BNO055(sensorID);
}

Gyro::~Gyro()
{
    
}

void Gyro::setup()
{
    if (!m_gyro.begin())
	{
		Serial.print("Gyro (BNO055) not detected");
		while (1);
	}
	
	m_gyro.setExtCrystalUse(true);
	m_timer.start();
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

int Gyro::size()
{
	return m_history.size();
}

void Gyro::record(sensors_event_t* point)
{
	if (size() > BUFFER_MAX)
		m_history.shift();
	m_history.add(*point);
}