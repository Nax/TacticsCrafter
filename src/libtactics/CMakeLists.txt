file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

# System-specific
file(GLOB_RECURSE SOURCES_WIN32 "sys/win32/*.cpp")
file(GLOB_RECURSE SOURCES_MACOS "sys/macos/*.cpp" "sys/macos/*.mm")
file(GLOB_RECURSE SOURCES_LINUX "sys/linux/*.cpp")
file(GLOB_RECURSE SOURCES_UNIX  "sys/unix/*.cpp")

list(REMOVE_ITEM SOURCES ${SOURCES_WIN32} ${SOURCES_MACOS} ${SOURCES_LINUX} ${SOURCES_UNIX})

if (WIN32)
    list(APPEND SOURCES ${SOURCES_WIN32})
elseif (APPLE)
    list(APPEND SOURCES ${SOURCES_MACOS} ${SOURCES_UNIX})
elseif(UNIX)
    list(APPEND SOURCES ${SOURCES_LINUX} ${SOURCES_UNIX})
endif()

add_library(libtactics SHARED ${SOURCES})
set_target_properties(libtactics PROPERTIES OUTPUT_NAME "libtactics" PREFIX "")

if (WIN32)
    target_compile_definitions(libtactics PRIVATE LIBTACTICS_EXPORT=1)
    target_link_libraries(libtactics Dirent::Dirent)
elseif (APPLE)
    target_link_libraries(libtactics "-framework Foundation")
endif()

target_link_libraries(libtactics Lua::Lua JsonCpp::JsonCpp)
