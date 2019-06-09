cmake_minimum_required(VERSION 3.13)
project(icebreaker)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES include/api/Wrapper.h src/Wrapper.cpp src/WrapperUtils.cpp include/api/models/User.h include/api/models/File.h include/api/models/Package.h include/api/models/Repository.h include/api/models/Response.hpp include/api/models/Entity.h)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(include/api)

add_library(icebreaker SHARED ${SOURCE_FILES})

target_include_directories(icebreaker PUBLIC include)
target_link_libraries(icebreaker Qt5::Core Qt5::Network)
