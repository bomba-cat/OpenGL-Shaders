cmake_minimum_required(VERSION 3.10)
project(shaderground C)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(SRC_FILES src/main.c src/glad.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g")

add_executable(${PROJECT_NAME} ${SRC_FILES})

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/shader" "${CMAKE_BINARY_DIR}/shader"
)

add_dependencies(shaderground copy_shaders)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
