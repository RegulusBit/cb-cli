cmake_minimum_required(VERSION 3.9)
project(bnb_cli)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(utilities)
add_subdirectory(libzmqm)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)
pkg_check_modules(JSONCPP jsoncpp)
## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )


add_executable(bnb_cli main.cpp)
target_include_directories(bnb_cli PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(bnb_cli utilities libzmqm ${ZeroMQ_LIBRARY} ${JSONCPP_LIBRARIES})


#  dependencies:
#  sudo apt-get install libjsoncpp-dev (cpp JSON Parser)
#  sudo apt-get install libzmq3-dev
#
#
