cmake_minimum_required(VERSION 3.22)

project(kikai-logger LANGUAGES CXX VERSION 0.1.0)

add_library(${PROJECT_NAME})

file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${SRC_FILES})
