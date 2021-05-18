set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
endif()

add_subdirectory(TacticsCrafter)
