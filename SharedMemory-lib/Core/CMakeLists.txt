cmake_minimum_required(VERSION 3.10)
project(transport.shrmem-Core VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(LIBPRINT_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBPRINT_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB LIBPRINT_LIB_HEADERS ${LIBPRINT_LIB_INCLUDE_DIR}/*.hpp)
file(GLOB LIBPRINT_LIB_SOURCE ${LIBPRINT_LIB_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${LIBPRINT_LIB_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBPRINT_LIB_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
