cmake_minimum_required(VERSION 3.13)
project(icebreaker)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES include/APIWrapper.h src/APIWrapper.cpp src/APIWrapperUtils.cpp include/models/User.h include/models/File.h include/models/Package.h include/models/Repository.h include/models/Response.hpp)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(include)

add_library(icebreaker SHARED ${SOURCE_FILES})

target_link_libraries(icebreaker Qt5::Core Qt5::Network)
