cmake_minimum_required(VERSION 3.16)
project(arm_tlm VERSION 23.10 LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED src/libarmaxi4.cpp src/libarmchi.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC SystemC::systemc)
