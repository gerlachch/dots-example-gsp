// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#include <SensorService.h>

using namespace dots::literals;

namespace examples
{
    SensorService::SensorService(std::string id, double px0) :
        m_rnd{ std::random_device{}()},
        m_px0(px0),
        m_config{
            .id = std::move(id),
            .enabled = true,
            .sensitivity = static_cast<dots::uint8_t>(std::min(100.0 * px0 * 1.2, 100.0))
        },
        m_sensor{
            .id = m_config.id,
            .activated = false
        },
        m_configSubscription{ dots::subscribe<SensorConfig>({ &SensorService::handleConfig, this }) },
        m_readTimer{ dots::create_timer(1s, { &SensorService::handleReadTimer, this }, true) }
    {
        dots::publish(m_config);
    }

    SensorService::~SensorService()
    {
        dots::remove(m_config);
        dots::remove(m_sensor);
    }

    void SensorService::handleConfig(const dots::Event<SensorConfig>& event)
    {
        if (const SensorConfig& config = event.updated(); config.id == m_config.id)
            m_config._merge(config);
    }

    void SensorService::handleReadTimer()
    {
        Sensor sensor{
            .activated = false
        };

        if (*m_config.enabled)
        {
            using dist_t = std::binomial_distribution<dots::uint16_t>;
            sensor.value = static_cast<dots::uint8_t>(dist_t{ 100, m_px0 }(m_rnd));

            if (*sensor.value > *m_config.sensitivity)
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
