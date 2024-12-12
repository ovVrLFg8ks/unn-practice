project(NamedPipeLibrary VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(PIPE_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PIPE_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(spdlog REQUIRED)

file(GLOB PIPE_LIB_HEADERS ${PIPE_LIB_INCLUDE_DIR}/*.hpp)
file(GLOB PIPE_LIB_SOURCE ${PIPE_LIB_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} SHARED ${PIPE_LIB_SOURCE} ${PIPE_LIB_HEADERS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")

target_include_directories(${PROJECT_NAME} PUBLIC ${PIPE_LIB_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})