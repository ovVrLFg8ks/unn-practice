add_library(NamedPipeLibrary
    src/NamedPipeTransport.cpp
    src/Protocol.cpp
    src/ClientApp.cpp
    src/ServerApp.cpp
)

# Подключение заголовков
target_include_directories(NamedPipeLibrary PUBLIC include)