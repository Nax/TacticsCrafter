include(ExternalProject)

set(DIRENT_VERSION            1.23.2)
set(DIRENT_INSTALL_DIR        "${CMAKE_BINARY_DIR}/_vendor")
set(DIRENT_INCLUDE_DIRECTORY  "${DIRENT_INSTALL_DIR}/include")

file(MAKE_DIRECTORY "${DIRENT_INCLUDE_DIRECTORY}")

ExternalProject_Add(
    ep_dirent
    URL             "https://github.com/tronkko/dirent/archive/refs/tags/${DIRENT_VERSION}.tar.gz"
    INSTALL_DIR     "${DIRENT_INSTALL_DIR}"
    CMAKE_ARGS
        "-DCMAKE_BUILD_TYPE=$<CONFIG>"
        "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DDIRENT_BUILD_TESTS=OFF"
)

add_library(Dirent::Dirent INTERFACE IMPORTED GLOBAL)
set_target_properties(
    Dirent::Dirent
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES   "${DIRENT_INCLUDE_DIRECTORY}"
)
add_dependencies(Dirent::Dirent ep_dirent)
