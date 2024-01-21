cmake_minimum_required(VERSION 3.26.4)
project(test_raylib_box2d)

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
    target_compile_options(${libName} PRIVATE "-w")
endfunction()


# Dependencies

# Add Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
add_git_dependency(
    raylib
    https://github.com/baz606/raylib.git
    5.0
)

# Get box2d
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
add_git_dependency(
    box2d
    https://github.com/baz606/box2d.git
    v2.4.1
)

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE box2d)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
