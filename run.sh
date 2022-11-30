#!/usr/bin/env bash

(
    export DOTS_LOG_LEVEL=5
    trap 'kill 0' SIGINT;
    ./_deps/dots-cpp-build/bin/dotsd/dotsd &
    sleep 1
    ./services/sensor-service/sensor-service "0.2" &
    ./services/alarm-service/alarm-service &
    ./services/hmi-service/hmi-service &
    wait
)
