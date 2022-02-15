add_subdirectory(lua)
add_subdirectory(jsoncpp)

if (WIN32)
    add_subdirectory(dirent)
endif()
