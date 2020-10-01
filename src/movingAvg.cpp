// Arduino Moving Average Library
// https://github.com/JChristensen/movingAvg
// Copyright (C) 2018 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#include <movingAvg.h>

// initialize - allocate the interval array
void movingAvg::begin()
{
    m_readings = new float[m_interval];
}

// add a new reading and return the new moving average
void movingAvg::reading(float newReading)
{
    // add each new data point to the sum until the m_readings array is filled
    if (m_nbrReadings < m_interval)
    {
        ++m_nbrReadings;
    }

    m_readings[m_next] = newReading;
    if (++m_next >= m_interval)
        m_next = 0;
}

// just return the current moving average
float movingAvg::getAvg()
{
    float sum = 0;
    const int readings = m_nbrReadings;
    int i;

    for (i = 0; i < readings; i++)
    {
        sum += m_readings[i];
    }
    return sum / m_nbrReadings;
}

// start the moving average over again
void movingAvg::reset()
{
    m_nbrReadings = 0;
    m_next = 0;
}