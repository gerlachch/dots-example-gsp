// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#include <fmt/format.h>
#include <DotsClearCache.dots.h>
#include <AlarmService.h>
#include <Alarm.dots.h>

using namespace dots::literals;

namespace examples
{
    AlarmService::AlarmService() :
        m_config{
            .enabled = true,
            .threshold = 3
        },
        m_configSubscription{ dots::subscribe<AlarmConfig>({ &AlarmService::handleConfig, this }) },
        m_sensorSubscription{ dots::subscribe<Sensor>({ &AlarmService::handleSensor, this }) }
    {
        dots::publish(m_config);
    }

    AlarmService::~AlarmService()
    {
        dots::remove(m_config);

        dots::publish(DotsClearCache{
            .typeNames = { dots::string_t{ Alarm::_Name } }
        });
    }

    void AlarmService::handleConfig(const dots::Event<AlarmConfig>& event)
    {
        const AlarmConfig& config = event.updated();
        m_config._merge(config);
    }

    void AlarmService::handleSensor(const dots::Event<Sensor>& event)
    {
        if (!*m_config.enabled)
            return;

        const Sensor& sensor = event.updated();
        uint32_t& activatedCount = m_activatedCounts[*sensor.id];

        if (*sensor.activated)
        {
            if (activatedCount < *m_config.threshold && ++activatedCount >= *m_config.threshold)
            {
                dots::publish(Alarm{
                    .id = dots::uuid_t::Random(),
                    .message = fmt::format("Alarm in '{}'", *sensor.id),
                    .time = dots::timepoint_t::Now()
                });
            }
        }
        else
            activatedCount = 0;
    }
}
