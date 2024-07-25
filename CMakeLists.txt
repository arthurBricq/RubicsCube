cmake_minimum_required( VERSION 3.5 )
project(Tuto1)
set(CMAKE_CXX_STANDARD 17)

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s WASM=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

if(NOT EMSCRIPTEN) 
    # not adding glfw when compiling with emscripten
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS} )
endif()

include_directories("./glad/include")
include_directories("./glm")

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_executable(Hello3D main3d.cpp glad/src/glad.c shader.cpp)
target_link_libraries(Hello3D ${OPENGL_LIBRARIES} glfw)