cmake_minimum_required(VERSION 3.10)
project(daemon VERSION "1.0.0" DESCRIPTION "Simple C++ template example for creating Linux daemons" LANGUAGES CXX)
include(GNUInstallDirs)

set(LIBPRINT_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB LIBPRINT_LIB_HEADERS ${LIBPRINT_LIB_INCLUDE_DIR}/*.hpp)

add_library(${PROJECT_NAME} SHARED ${LIBPRINT_LIB_INCLUDE_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17) # update your C++ version here if you like
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPRINT_LIB_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} LINKER_LANGUAGE CXX)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
