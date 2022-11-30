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
        m_activatedCount(0),
        m_sensorSubscription{ dots::subscribe<Sensor>({ &AlarmService::handleSensor, this }) }
    {
        /* do nothing */
    }

    AlarmService::~AlarmService()
    {
        dots::publish(DotsClearCache{
            .typeNames = { dots::string_t{ Alarm::_Name } }
        });
    }

    void AlarmService::handleSensor(const dots::Event<Sensor>& event)
    {
        const Sensor& sensor = event.updated();
        uint32_t& activatedCount = m_activatedCount;

        if (*sensor.activated)
        {
            if (activatedCount < 3 && ++activatedCount >= 3)
            {
                dots::publish(Alarm{
                    .message = fmt::format("Alarm in 'Lower Hallway'"),
                    .time = dots::timepoint_t::Now()
                });
            }
        }
        else
            activatedCount = 0;
    }
}
