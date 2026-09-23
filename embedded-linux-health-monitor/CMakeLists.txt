cmake_minimum_required(VERSION 3.15)
project(embedded-linux-health-monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MONITOR_SOURCES
    src/cpu_monitor.cpp
    src/memory_monitor.cpp
    src/disk_monitor.cpp
    src/temperature_monitor.cpp
    src/network_monitor.cpp
    src/service_monitor.cpp
    src/logger.cpp
    src/config_manager.cpp
)

add_executable(health_monitor
    src/main.cpp
    ${MONITOR_SOURCES}
)

target_include_directories(health_monitor PUBLIC
    include/
)

option(BUILD_TESTS "Build test executables" OFF)
if(BUILD_TESTS)
    enable_testing()

    add_executable(test_cpu tests/test_cpu.cpp src/cpu_monitor.cpp)
    target_include_directories(test_cpu PUBLIC include/)

    add_executable(test_memory tests/test_memory.cpp src/memory_monitor.cpp)
    target_include_directories(test_memory PUBLIC include/)

    add_executable(test_config tests/test_config.cpp src/config_manager.cpp)
    target_include_directories(test_config PUBLIC include/)

    add_executable(test_service_monitor tests/test_service_monitor.cpp src/service_monitor.cpp)
    target_include_directories(test_service_monitor PUBLIC include/)

    add_test(NAME test_cpu COMMAND test_cpu WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_test(NAME test_memory COMMAND test_memory WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_test(NAME test_config COMMAND test_config WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_test(NAME test_service_monitor COMMAND test_service_monitor WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()