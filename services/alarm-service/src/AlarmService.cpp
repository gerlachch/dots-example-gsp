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
        m_resetSubscription{ dots::subscribe<AlarmReset>({ &AlarmService::handleReset, this }) },
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

    void AlarmService::handleReset(const dots::Event<AlarmReset>& event)
    {
        if (const AlarmReset& alarmReset = event.transmitted(); alarmReset.alarms.isValid())
        {
            for (const dots::uuid_t& alarmId : *alarmReset.alarms)
            {
                if (auto* alarm = dots::container<Alarm>().find({ .id = alarmId }))
                {
                    dots::remove(*alarm);
                }
            }
        }
        else
        {
            dots::publish(DotsClearCache{
                .typeNames = { dots::string_t{ Alarm::_Name } }
            });
            m_activatedCounts.clear();
        }
    }

    void AlarmService::handleSensor(const dots::Event<Sensor>& event)
    {
        if (!*m_config.enabled)
        {
            return;
        }

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
        {
            activatedCount = 0;
        }
    }
}
