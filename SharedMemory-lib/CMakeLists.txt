cmake_minimum_required(VERSION 3.10)
project(transport.shrmem VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

add_subdirectory(Core)
add_subdirectory(Server)
add_subdirectory(Client)

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/Server/include/Server.hpp ${CMAKE_CURRENT_SOURCE_DIR}/Client/include/Client.hpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

target_link_libraries(${PROJECT_NAME} PUBLIC transport.shrmem-Server)
target_link_libraries(${PROJECT_NAME} PUBLIC transport.shrmem-Client)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Core/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
