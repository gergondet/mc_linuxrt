cmake_minimum_required(VERSION 3.5)

project(mc_linuxrt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(mc_rtc REQUIRED)
find_package(mc_udp REQUIRED)

add_library(MCLinuxRTMainLoop OBJECT src/main.cpp src/thread.h src/timer.cpp src/timer.h)

add_executable(MCLinuxRTServer src/server.cpp $<TARGET_OBJECTS:MCLinuxRTMainLoop>)
target_link_libraries(MCLinuxRTServer PUBLIC mc_udp::server Threads::Threads)

add_executable(MCLinuxRT src/native.cpp $<TARGET_OBJECTS:MCLinuxRTMainLoop>)
target_link_libraries(MCLinuxRT PUBLIC mc_rtc::mc_control Threads::Threads)

add_executable(MCLinuxRTMeasureLatency src/latency.cpp $<TARGET_OBJECTS:MCLinuxRTMainLoop>)
target_link_libraries(MCLinuxRTMeasureLatency PUBLIC Threads::Threads)

install(TARGETS MCLinuxRTServer MCLinuxRT RUNTIME DESTINATION bin)
