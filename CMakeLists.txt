cmake_minimum_required(VERSION 3.3)

project(vulkan_ex)

# Global CMake options:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json

set(src_files
    main.cpp

    src/init_core.cpp

    src/lib_wrappers/tiny_obj_loader_wrap.cpp
    src/lib_wrappers/stb_wrap.cpp
)

# Create the executable:
add_executable(${PROJECT_NAME} ${src_files})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add compiler options:
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-function)

# Add compiler definitions:
target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DASSETS_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets/"
)

# Post build script to build shaders:

# Copy assets tp build directory:
add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets)

# Compile shaders:
add_custom_command(
    TARGET ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders
    PRE_BUILD COMMAND /bin/bash
    ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders/compile_shaders.sh
)

# Core library:
target_link_libraries(${PROJECT_NAME} PRIVATE core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/core)

# stb_image library:
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/include)

# Tiny OBJ Loader library:
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader/include)

# fmt library:
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt/include)

# glfw library:
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include/GLFW)

# glm library:
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)

# vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
