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

# Install
if (WIN32)
    install(TARGETS TacticsCrafter RUNTIME DESTINATION .)
    install(FILES "$<TARGET_FILE:Lua::Lua>" "$<TARGET_FILE:JsonCpp::JsonCpp>" DESTINATION .)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/data" DESTINATION .)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
    include(InstallRequiredSystemLibraries)
    install(CODE "execute_process(COMMAND \"${QT_DEPLOY}\" \"--release\" \"\${CMAKE_INSTALL_PREFIX}/TacticsCrafter.exe\" \"--no-compiler-runtime\" \"--no-opengl-sw\" \"--no-system-d3d-compiler\")")
elseif(APPLE)
    install(TARGETS TacticsCrafter BUNDLE DESTINATION .)
    set(APP "\${CMAKE_INSTALL_PREFIX}/TacticsCrafter.app")
    #install(CODE "
    #execute_process(COMMAND \"${QT_DEPLOY}\" \"${APP}\" \"-verbose=1\")
    #execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"$<TARGET_FILE:libtactics>\" \"${APP}/Contents/Frameworks/\")
    #")
    install(CODE "
    execute_process(COMMAND \"${QT_DEPLOY}\" \"${APP}\" \"-verbose=1\")
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"$<TARGET_FILE:libtactics>\" \"$<TARGET_FILE:Lua::Lua>\" \"$<TARGET_FILE:JsonCpp::JsonCpp>\" \"${APP}/Contents/Frameworks/\")
    ")
endif()
