cmake_minimum_required(VERSION 3.14)
project(TacticsCrafter)

# Version
file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_RAW)
string(REGEX MATCH "([0-9]+)\.([0-9]+)\.([0-9]+)" _ "${VERSION_RAW}")
set(VERSION_MAJOR ${CMAKE_MATCH_1})
set(VERSION_MINOR ${CMAKE_MATCH_2})
set(VERSION_PATCH ${CMAKE_MATCH_3})
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(NIGHTLY "Append the commit short hash to the version" OFF)
if (NIGHTLY)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE VERSION_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(VERSION "${VERSION}-${VERSION_COMMIT}")
endif()

message(STATUS "Version: ${VERSION}")

# Global CMake settings
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(vendor)
add_subdirectory(src)

# CPack
set(CPACK_PACKAGE_NAME "TacticsCrafter")
set(CPACK_PACKAGE_VENDOR "Maxime Bacoux")
set(CPACK_PACKAGE_CONTACT "Maxime Bacoux")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/pkg")

set(CPACK_BUILD_CONFIG "Release")
if (WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_SYSTEM_NAME "mac")
elseif(UNIX)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libgcc1, libstdc++6, liblua5, libqt5core5a, libqt5gui5, libqt5widgets5")
    set(CPACK_GENERATOR "DEB")
endif()
include(CPack)
