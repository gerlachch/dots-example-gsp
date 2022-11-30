// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#pragma once
#include <map>
#include <dots/dots.h>
#include <Sensor.dots.h>
#include <AlarmConfig.dots.h>

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

        void handleConfig(const dots::Event<AlarmConfig>& event);
        void handleSensor(const dots::Event<Sensor>& event);
        
        std::map<dots::string_t, uint32_t> m_activatedCounts;
        AlarmConfig m_config;
        dots::Subscription m_configSubscription;
        dots::Subscription m_sensorSubscription;
    };
}
