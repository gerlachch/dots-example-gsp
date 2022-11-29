#!/usr/bin/env bash

(
    export DOTS_LOG_LEVEL=5
    trap 'kill 0' SIGINT;
    ./_deps/dots-cpp-build/bin/dotsd/dotsd &
    sleep 1
    ./services/sensor-service/sensor-service "Lower Hallway" "0.2" &
    ./services/sensor-service/sensor-service "Upper Hallway" "0.4" &
    ./services/sensor-service/sensor-service "Staircase" "0.6" &
    ./services/sensor-service/sensor-service "Office" "0.2" &
    ./services/sensor-service/sensor-service "Basement" "0.8" &
    ./services/alarm-service/alarm-service &
    ./services/hmi-service/hmi-service &
    wait
)
