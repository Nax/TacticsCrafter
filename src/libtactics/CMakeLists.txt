file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(libtactics SHARED ${SOURCES})
set_target_properties(libtactics PROPERTIES OUTPUT_NAME "libtactics")

if (WIN32)
    target_compile_definitions(libtactics PRIVATE LIBTACTICS_EXPORT=1)
endif()

target_link_libraries(libtactics Lua::Lua)
