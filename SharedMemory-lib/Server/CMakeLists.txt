cmake_minimum_required(VERSION 3.10)
project(transport.shrmem-Server VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(SERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SERVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SERVER_HEADERS ${SERVER_INCLUDE_DIR}/*.hpp)
file(GLOB SERVER_SOURCE ${SERVER_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SERVER_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

target_link_libraries (${PROJECT_NAME} PUBLIC transport.shrmem-Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${SERVER_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
