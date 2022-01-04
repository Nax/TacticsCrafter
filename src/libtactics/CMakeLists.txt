file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

# System-specific
file(GLOB_RECURSE SOURCES_WIN32 "sys/win32/*.cpp")
file(GLOB_RECURSE SOURCES_MACOS "sys/macos/*.cpp")
file(GLOB_RECURSE SOURCES_LINUX "sys/linux/*.cpp")

list(REMOVE_ITEM SOURCES ${SOURCES_WIN32} ${SOURCES_MACOS} ${SOURCES_LINUX})

if (WIN32)
    list(APPEND SOURCES ${SOURCES_WIN32})
elseif (APPLE)
    list(APPEND SOURCES ${SOURCES_MACOS})
elseif(UNIX)
    list(APPEND SOURCES ${SOURCES_LINUX})
endif()

add_library(libtactics SHARED ${SOURCES})
set_target_properties(libtactics PROPERTIES OUTPUT_NAME "libtactics")

if (WIN32)
    target_compile_definitions(libtactics PRIVATE LIBTACTICS_EXPORT=1)
endif()

target_link_libraries(libtactics Lua::Lua JsonCpp::JsonCpp)
