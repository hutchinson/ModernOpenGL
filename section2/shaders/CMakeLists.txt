cmake_minimum_required(VERSION 3.8.2)

project(glfw_shaders)
set(CMAKE_CXX_STANDARD 11)

# Include Thirdparty Libraries
set(CMAKE_PREFIX_PATH /usr/local)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
