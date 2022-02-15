find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
if (WIN32 OR APPLE)
    find_package(QtDeploy REQUIRED)
endif()

set(CMAKE_AUTOMOC ON)

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
endif()

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
if (WIN32)
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/res/app.rc")
elseif(APPLE)
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/res/icon.icns")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/res/icon.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

add_executable(TacticsCrafter WIN32 MACOSX_BUNDLE ${SOURCES})
target_include_directories(TacticsCrafter PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(TacticsCrafter Qt5::Widgets libtactics)

if (WIN32)
    add_custom_command(
        TARGET TacticsCrafter POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:Lua::Lua>" "$<TARGET_FILE:JsonCpp::JsonCpp>" "$<TARGET_FILE_DIR:TacticsCrafter>"
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

if (APPLE)
    set(DATA_DEST "$<TARGET_BUNDLE_CONTENT_DIR:TacticsCrafter>/Resources")
else()
    set(DATA_DEST "$<TARGET_FILE_DIR:TacticsCrafter>")
endif()

add_custom_command(
    TARGET TacticsCrafter POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/data" "${DATA_DEST}/data"
    COMMENT "Copying data"
    VERBATIM
)
