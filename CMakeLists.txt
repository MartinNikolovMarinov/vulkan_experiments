cmake_minimum_required(VERSION 3.3)

project(vulkan_ex)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
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
