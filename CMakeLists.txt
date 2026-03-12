cmake_minimum_required(VERSION 3.16)
project(arm_tlm VERSION 23.10 LANGUAGES CXX)

add_library(armtlmaxi4 SHARED src/libarmaxi4.cpp)
target_include_directories(armtlmaxi4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(armtlmaxi4 PUBLIC SystemC::systemc)

add_library(armtlmchi SHARED src/libarmchi.cpp)
target_include_directories(armtlmchi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(armtlmchi PUBLIC SystemC::systemc)

add_library(arm_tlm INTERFACE)
target_link_libraries(arm_tlm INTERFACE armtlmaxi4 armtlmchi)
