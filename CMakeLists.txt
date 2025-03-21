cmake_minimum_required(VERSION 3.10)
project(shaderground C)

find_package(OpenGL REQUIRED)

set(SRC_FILES src/main.c src/glad.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRC_FILES})

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/shader" "${CMAKE_BINARY_DIR}/shader"
)

add_dependencies(shaderground copy_shaders)

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
