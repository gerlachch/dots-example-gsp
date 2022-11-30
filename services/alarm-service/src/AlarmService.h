// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#pragma once
#include <map>
#include <dots/dots.h>
#include <Sensor.dots.h>

namespace examples
{
    struct AlarmService
    {
        AlarmService();
        AlarmService(const AlarmService& other) = delete;
        AlarmService(AlarmService&& other) = default;
        ~AlarmService();

        AlarmService& operator = (const AlarmService& rhs) = delete;
        AlarmService& operator = (AlarmService&& rhs) = default;

    private:

        void handleSensor(const dots::Event<Sensor>& event);
        
        uint32_t m_activatedCount;
        dots::Subscription m_sensorSubscription;
    };
}
