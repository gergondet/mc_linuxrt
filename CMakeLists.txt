cmake_minimum_required(VERSION 3.5)

project(mc_linuxrt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(mc_rtc REQUIRED)
find_package(mc_udp REQUIRED)

add_library(MCLinuxRTMainLoopObjects OBJECT src/main.cpp src/thread.h)
add_library(MCLinuxRTMainLoop INTERFACE)
target_link_libraries(MCLinuxRTMainLoop INTERFACE Threads::Threads $<TARGET_OBJECTS:MCLinuxRTMainLoopObjects>)

add_executable(MCLinuxRTServer src/server.cpp)
target_link_libraries(MCLinuxRTServer PUBLIC MCLinuxRTMainLoop mc_udp::server)

add_executable(MCLinuxRT src/native.cpp)
target_link_libraries(MCLinuxRT PUBLIC MCLinuxRTMainLoop mc_rtc::mc_control)

add_executable(MCLinuxRTMeasureLatency src/latency.cpp)
target_link_libraries(MCLinuxRTMeasureLatency PUBLIC MCLinuxRTMainLoop)

install(TARGETS MCLinuxRTServer MCLinuxRT RUNTIME DESTINATION bin)
