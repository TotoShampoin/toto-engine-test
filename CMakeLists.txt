cmake_minimum_required(VERSION 3.12)
project(cppgl)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dist)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/toto-engine)

file(GLOB SOURCES ${SRC_DIR}/*.cpp)

add_executable(cppgl ${SOURCES})

target_include_directories(cppgl PUBLIC ${INC_DIR})

target_link_libraries(cppgl PRIVATE toto-engine)
