cmake_minimum_required(VERSION 3.14)
project(Day08)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT "/home/patryk/libs/boost_1_70_0")

message(STATUS "start running cmake...")
find_package(Boost 1.70.0 COMPONENTS REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(Day08 main.cpp)

if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} stdc++fs)


