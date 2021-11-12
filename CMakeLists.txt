cmake_minimum_required(VERSION 3.20)
project(gym)

set(CMAKE_CXX_STANDARD 11)

add_executable(gym src/main.cpp src/Customer.cpp src/Studio.cpp src/Action.cpp src/Workout.cpp)
