cmake_minimum_required(VERSION 3.10)
project(gtkmm-helper)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

find_package(Boost 1.42 COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(gtkmm-helper ${SOURCES})
target_link_libraries(gtkmm-helper ${Boost_LIBRARIES})

install(TARGETS gtkmm-helper DESTINATION bin)

