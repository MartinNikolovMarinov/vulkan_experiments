cmake_minimum_required(VERSION 3.3)

project(vulkan_ex)

# Global CMake options:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json

set(src_files
    main.cpp

    src/init_core.cpp
)

# Create the executable:
add_executable(${PROJECT_NAME} ${src_files})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add compiler options:
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wno-unknown-pragmas -Wno-unused-function)

# Add compiler definitions:
target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

# Core library:
target_link_libraries(${PROJECT_NAME} PRIVATE core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/core)

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
