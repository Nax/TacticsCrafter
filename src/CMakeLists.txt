set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# RPATH
set(CMAKE_MACOSX_RPATH      TRUE)
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_RPATH       "${CMAKE_BINARY_DIR}/third_party/lib")

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/include")

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
endif()

add_subdirectory(libtactics)
add_subdirectory(TacticsCrafter)
add_subdirectory(tcraft)
