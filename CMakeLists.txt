cmake_minimum_required(VERSION 3.22)

project(kikai-utils LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCE_FILES src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
