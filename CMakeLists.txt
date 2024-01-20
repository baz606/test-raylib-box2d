cmake_minimum_required(VERSION 3.26)
project(test_raylib_box2d)

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/baz606/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Get raygui
set(RAYLIB_GUI_VERSION 4.0)
find_package(raygui ${RAYLIB_GUI_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raygui_FOUND) # If there's none, fetch raygui
    include(FetchContent)
    FetchContent_Declare(
            raygui
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/baz606/raygui/archive/refs/tags/${RAYLIB_GUI_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raygui)
    if (NOT raygui_POPULATED) # Have we downloaded raygui yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raygui)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        # Copy over raygui.h over to raylib_SOURCE_DIR
        configure_file(
                ${raygui_SOURCE_DIR}/src/raygui.h ${raylib_SOURCE_DIR}/src
        )
    endif()
endif()

# Get box2d
set(BOX2D_VERSION 2.4.1)
find_package(box2d ${BOX2D_VERSION} QUIET) # QUIET or REQUIRED
if (NOT box2d_FOUND) # If there's none, fetch raygui
    include(FetchContent)
    FetchContent_Declare(
            box2d
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/baz606/box2d/archive/refs/tags/v${BOX2D_VERSION}.tar.gz
    )
    FetchContent_GetProperties(box2d)
    if (NOT box2d_POPULATED) # Have we downloaded raygui yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(box2d)
    endif()
endif()

set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
add_subdirectory(${box2d_SOURCE_DIR} ${box2d_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib box2d)

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
