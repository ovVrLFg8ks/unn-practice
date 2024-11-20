cmake_minimum_required(VERSION 3.10)
project(transport.shrmem-Client VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(CLIENT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CLIENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB CLIENT_HEADERS ${CLIENT_INCLUDE_DIR}/*.hpp)
file(GLOB CLIENT_SOURCE ${CLIENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CLIENT_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

target_link_libraries (${PROJECT_NAME} PUBLIC transport.shrmem-Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${CLIENT_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
