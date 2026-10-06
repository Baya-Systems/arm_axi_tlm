cmake_minimum_required(VERSION 3.16)
project(arm_tlm VERSION 23.10 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# A parent project that pulled this in with add_subdirectory/FetchContent has
# already defined SystemC::systemc, so this is a no-op there. Standalone -- which
# is what building an installable package requires -- it finds one instead of
# failing at the first target_link_libraries below.
if(NOT TARGET SystemC::systemc)
    find_package(SystemCLanguage 3.0 REQUIRED CONFIG)
endif()

add_library(armtlmaxi4 SHARED src/libarmaxi4.cpp)
target_include_directories(armtlmaxi4 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(armtlmaxi4 PUBLIC SystemC::systemc)

add_library(armtlmchi SHARED src/libarmchi.cpp)
target_include_directories(armtlmchi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(armtlmchi PUBLIC SystemC::systemc)

add_library(arm_tlm INTERFACE)
target_link_libraries(arm_tlm INTERFACE armtlmaxi4 armtlmchi)

# Namespaced aliases so a consumer can link ARM::arm_tlm whichever way it got
# here. They also fail at configure time when absent, where a bare name would
# reach the linker as -larm_tlm and fail much later with a worse message.
add_library(ARM::armtlmaxi4 ALIAS armtlmaxi4)
add_library(ARM::armtlmchi ALIAS armtlmchi)
add_library(ARM::arm_tlm ALIAS arm_tlm)

# ARM's example traffic generators, memories, monitors and transactors, as
# libraries a consumer can build on rather than copy. Their headers are staged
# under ARM/TLM/examples/<protocol>/ so a consumer includes them by one path
# whether it builds against this tree or an install. The *Example.cpp sc_main
# programs stay out; the Makefiles beside them still build those.
set(_examples_inc ${CMAKE_CURRENT_BINARY_DIR}/examples_include)
foreach(_proto chi axi)
    file(GLOB _hdrs ${CMAKE_CURRENT_SOURCE_DIR}/examples/${_proto}/*.h)
    foreach(_hdr ${_hdrs})
        get_filename_component(_name ${_hdr} NAME)
        configure_file(${_hdr} ${_examples_inc}/ARM/TLM/examples/${_proto}/${_name} COPYONLY)
    endforeach()
endforeach()

add_library(armtlmchi_examples SHARED
    examples/chi/CHIMemory.cpp
    examples/chi/CHIMonitor.cpp
    examples/chi/CHITrafficGenerator.cpp)
target_include_directories(armtlmchi_examples PUBLIC
    $<BUILD_INTERFACE:${_examples_inc}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(armtlmchi_examples PUBLIC armtlmchi)

add_library(armtlmaxi4_examples SHARED
    examples/axi/AXIMemory.cpp
    examples/axi/AXIMonitor.cpp
    examples/axi/AXITrafficGenerator.cpp
    examples/axi/AXITransactors.cpp)
target_include_directories(armtlmaxi4_examples PUBLIC
    $<BUILD_INTERFACE:${_examples_inc}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(armtlmaxi4_examples PUBLIC armtlmaxi4)

add_library(ARM::armtlmchi_examples ALIAS armtlmchi_examples)
add_library(ARM::armtlmaxi4_examples ALIAS armtlmaxi4_examples)

# Installable package, so a consumer can build against one prebuilt copy instead
# of compiling this per project. Nothing above changes for existing
# add_subdirectory/FetchContent consumers: BUILD_INTERFACE is exactly the include
# path they already saw, and INSTALL_INTERFACE is empty for them.
install(TARGETS armtlmaxi4 armtlmchi arm_tlm armtlmchi_examples armtlmaxi4_examples
        EXPORT arm_tlmTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${_examples_inc}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT arm_tlmTargets
        NAMESPACE ARM::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arm_tlm)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm_tlmConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/arm_tlmConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arm_tlm)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/arm_tlmConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/arm_tlmConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/arm_tlmConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arm_tlm)
