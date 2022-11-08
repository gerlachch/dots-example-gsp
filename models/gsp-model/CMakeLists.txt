cmake_minimum_required(VERSION 3.19)
project(gsp-model LANGUAGES CXX)
set(TARGET_NAME GspModel)
set(EXPORT_NAME GspModel)

# dependencies
#find_package(DOTS REQUIRED) (uncomment when dependency is not part of build tree)

# target
add_library(${TARGET_NAME} INTERFACE)
add_library(${EXPORT_NAME}::${EXPORT_NAME} ALIAS ${TARGET_NAME})

# properties
target_dots_model(${TARGET_NAME}
    src/alarm.dots
    src/sensor.dots
)
target_include_directories(${TARGET_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_compile_features(${TARGET_NAME}
    INTERFACE
        cxx_std_20
)
target_link_libraries(${TARGET_NAME}
    INTERFACE
        DOTS::DOTS
)
