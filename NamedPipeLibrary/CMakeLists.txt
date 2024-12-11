cmake_minimum_required(VERSION 3.15)
project(NamedPipeLibrary)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем spdlog как зависимость
find_package(spdlog REQUIRED)

# Добавление библиотеки
add_library(NamedPipeLibrary
    src/ClientApp.cpp
    src/NamedPipeTransport.cpp
    src/Protocol.cpp
    src/ServerApp.cpp
)

# Указываем заголовочные файлы
target_include_directories(NamedPipeLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линкуем spdlog
target_link_libraries(NamedPipeLibrary PRIVATE spdlog::spdlog)

# Устанавливаем пути для установки библиотеки
install(TARGETS NamedPipeLibrary
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)