// SPDX-License-Identifier: LGPL-3.0-only
// Copyright 2022 Christopher Gerlach <gerlachch@gmx.com>
#pragma once
#include <random>
#include <dots/dots.h>
#include <SensorConfig.dots.h>
#include <Sensor.dots.h>

namespace examples
{
    struct SensorService
    {
        SensorService(std::string id, double px0);
        SensorService(const SensorService& other) = delete;
        SensorService(SensorService&& other) = default;
        ~SensorService();

        SensorService& operator = (const SensorService& rhs) = delete;
        SensorService& operator = (SensorService&& rhs) = default;

    private:

        using rnd_t = std::mt19937_64;

        void handleConfig(const dots::Event<SensorConfig>& event);
        void handleReadTimer();

        rnd_t m_rnd;
        double m_px0;
        SensorConfig m_config;
        Sensor m_sensor;
        dots::Subscription m_configSubscription;
        dots::Timer m_readTimer;
    };
}
