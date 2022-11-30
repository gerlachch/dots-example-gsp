// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#include <SensorService.h>

using namespace dots::literals;

namespace examples
{
    SensorService::SensorService(double px0) :
        m_rnd{ std::random_device{}()},
        m_px0(px0),
        m_sensitivity(25),
        m_sensor{
            .activated = false
        },
        m_readTimer{ dots::create_timer(1s, { &SensorService::handleReadTimer, this }, true) }
    {
        /* do nothing */
    }

    SensorService::~SensorService()
    {
        dots::remove(m_sensor);
    }

    void SensorService::handleReadTimer()
    {
        Sensor sensor{
            .activated = false
        };

        {
            using dist_t = std::binomial_distribution<dots::uint16_t>;
            sensor.value = static_cast<dots::uint8_t>(dist_t{ 100, m_px0 }(m_rnd));

            if (*sensor.value > m_sensitivity)
            {
                sensor.activated = true;
                sensor.lastActivation = dots::timepoint_t::Now();
            }
        }

        if (dots::property_set_t diff = m_sensor._diffProperties(sensor, sensor._validProperties()))
        {
            m_sensor._merge(sensor, diff);
            dots::publish(m_sensor);
        }
    }
}
