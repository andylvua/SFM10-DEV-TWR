cmake_minimum_required(VERSION 3.8.2)

project(Reader)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

add_executable(reader reader.cpp)

find_package(Qt6 COMPONENTS Core SerialPort Widgets Charts REQUIRED)
target_link_libraries(reader Qt6::Core Qt6::SerialPort Qt6::Widgets Qt6::Charts)

target_include_directories(reader PRIVATE ../shared)

