cmake_minimum_required(VERSION 3.16)
project(arm_tlm VERSION 23.10 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE SystemC::systemc)