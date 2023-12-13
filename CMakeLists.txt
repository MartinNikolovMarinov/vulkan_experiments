cmake_minimum_required(VERSION 3.3)

project(graphics_programming_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json

# Functions

function(init_target target)
    target_include_directories(${target}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    # Add compiler options:
    target_compile_options(${target} PRIVATE
        -Wall -Wextra
        -Wno-unknown-pragmas -Wno-unused-function
    )

    # Add compiler definitions:
    target_compile_definitions(${target} PRIVATE
        -DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets/${target}/"
    )

    # Post build scripts for assets.

    # Copy assets to build directory:
    add_custom_command(TARGET ${target}
        POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/${target}
        ${CMAKE_CURRENT_BINARY_DIR}/assets/${target}
    )

    # Compile shaders:
    add_custom_command(
        TARGET ${target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/${target}/shaders
        POST_BUILD COMMAND /bin/bash
        ${CMAKE_CURRENT_BINARY_DIR}/assets/${target}/shaders/compile_shaders.sh
    )
endfunction()

function(link_dependencies target)
    # Core library:
    target_link_libraries(${target} PRIVATE core)

    # stb_image library:
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/include)

    # Tiny OBJ Loader library:
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/include)

    # fmt library:
    target_link_libraries(${target} PRIVATE fmt)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt/include)

    # glfw library:
    target_link_libraries(${target} PRIVATE glfw)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include/GLFW)

    # glm library:
    target_link_libraries(${target} PRIVATE glm)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)

    # vulkan
    find_package(Vulkan REQUIRED)
    target_link_libraries(${target} PRIVATE Vulkan::Vulkan)
    target_include_directories(${target} PRIVATE ${Vulkan_INCLUDE_DIRS})
endfunction()

# Add CMake subdirectories

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)

# Default source files

set(COMMON_SOURCES
    src/init_core.cpp

    src/lib_wrappers/stb_wrap.cpp
    src/lib_wrappers/tiny_obj_loader_wrap.cpp
)

# Create targets

add_executable(ex_01 ex_01.cpp ${COMMON_SOURCES})
add_executable(ex_02 ex_02.cpp ${COMMON_SOURCES})

# Setup targets

init_target(ex_01)
init_target(ex_02)

# Link dependencies

link_dependencies(ex_01)
link_dependencies(ex_02)
