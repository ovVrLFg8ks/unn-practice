cmake_minimum_required(VERSION 3.10)
project(fault VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

set(DAEMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DAEMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB DAEMON_HEADERS ${DAEMON_INCLUDE_DIR}/*.hpp)
file(GLOB DAEMON_SOURCE ${DAEMON_SOURCE_DIR}/*.cpp)

set(DAEMON_HEADERS_LIST ${DAEMON_INCLUDE_DIR})

include_directories(${DAEMON_HEADERS_LIST})

add_executable(${PROJECT_NAME} ${DAEMON_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")

target_link_libraries (${PROJECT_NAME} PRIVATE daemon)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# Install the config file .conf
install(FILES ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.conf DESTINATION /etc/${PROJECT_NAME}/)
# Install the systemd file .service
install(FILES ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.service DESTINATION /etc/systemd/system/)
# Install the binary program
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)