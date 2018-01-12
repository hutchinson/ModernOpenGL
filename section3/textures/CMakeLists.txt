cmake_minimum_required(VERSION 3.8.2)

project(textures)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(HEADERS Shader.h
            stb_image.h)

set(SOURCES main.cpp
            stb_image.cpp
            Shader.cpp)

configure_file(SimpleVShader.glsl SimpleVShader.glsl)
configure_file(MultiColourFragShader.glsl MultiColourFragShader.glsl)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Ensure our textures are copied over.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/container.jpg
            ${CMAKE_CURRENT_BINARY_DIR}/container.jpg)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/awesomeface.png
            ${CMAKE_CURRENT_BINARY_DIR}/awesomeface.png)

# Include Thirdparty Libraries
set(CMAKE_PREFIX_PATH /usr/local)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
endif()

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
endif()

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
if(GLFW_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})

    # Note: the path currently returned by PkgConfig for GLFW isn't the full path
    # so we construct it here so the macOS linker can find it.
    if(APPLE)
        set(GLFW_LIBRARIES "${GLFW_LIBRARY_DIRS}/lib${GLFW_LIBRARIES}.dylib")
    endif()
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
endif()
