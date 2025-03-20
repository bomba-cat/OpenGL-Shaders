cmake_minimum_required(VERSION 3.10)
project(shader C)

find_package(OpenGL REQUIRED)

set(SRC_FILES src/main.c src/glad.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
