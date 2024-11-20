cmake_minimum_required(VERSION 3.10)

add_subdirectory(daemon-lib)
add_subdirectory(socket-lib)
add_subdirectory(SharedMemory-lib)
add_subdirectory(configuration)
add_subdirectory(radio)
add_subdirectory(fault)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall daemon files")
add_custom_command(
        TARGET "uninstall"
        POST_BUILD
        COMMENT "Uninstall files with install_manifest.txt"
        COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in install_manifest.txt to be uninstalled!
)