#
# Copyright 2019-2020 mc_rtc development team
#

cmake_minimum_required(VERSION 3.5)

project(mc_linuxrt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_RT_SERVER "Build RT server" ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(mc_rtc REQUIRED)

if(BUILD_RT_SERVER)
  find_package(mc_udp REQUIRED)
endif()

add_library(MCLinuxRTMainLoop OBJECT src/main.cpp src/thread.h src/timer.cpp src/timer.h)

function(add NAME SRC)
  add_executable(${NAME} ${SRC} $<TARGET_OBJECTS:MCLinuxRTMainLoop>)
  target_link_libraries(${NAME} PUBLIC Threads::Threads ${ARGN})
  install(TARGETS ${NAME} RUNTIME DESTINATION bin)
endfunction()

add(MCLinuxRT src/native.cpp mc_rtc::mc_control)
add(MCLinuxRTMeasureLatency src/latency.cpp)

if(BUILD_RT_SERVER)
  add(MCLinuxRTServer src/server.cpp mc_udp::server)
endif()
