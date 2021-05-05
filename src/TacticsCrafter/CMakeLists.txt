find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
if (WIN32 OR APPLE)
    find_package(QtDeploy REQUIRED)
endif()

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
add_executable(TacticsCrafter WIN32 MACOSX_BUNDLE ${SOURCES})
target_include_directories(TacticsCrafter PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(TacticsCrafter Lua::Lua Qt5::Widgets)

if (WIN32)
    add_custom_command(
        TARGET TacticsCrafter POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Lua::Lua>" "$<TARGET_FILE_DIR:TacticsCrafter>"
        COMMENT "Copying DLLs"
        VERBATIM
    )
    add_custom_command(
        TARGET TacticsCrafter POST_BUILD
        COMMAND "${QT_DEPLOY}" "$<TARGET_FILE:TacticsCrafter>" "--no-compiler-runtime" "--no-opengl-sw" "--no-system-d3d-compiler"
        COMMENT "Deploying Qt"
        VERBATIM
    )
endif()
