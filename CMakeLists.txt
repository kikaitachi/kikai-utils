cmake_minimum_required(VERSION 3.22)

project(kikai-logger LANGUAGES CXX VERSION 0.1.0)

add_library(${PROJECT_NAME} src/logger.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_CXX_STANDARD 20
  CMAKE_CXX_STANDARD_REQUIRED ON
  CMAKE_CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
