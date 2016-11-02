cmake_minimum_required (VERSION 2.6)
project (OpenGLBasic)

set(CMAKE_BUILD_TYPE debug)

include_directories("/usr/include/SDL2/")

add_executable(OpenGLBasic Box.cpp Face.cpp Main.cpp Math.cpp Light.cpp Timer.cpp Camera.cpp EAngle.cpp Shader.cpp Vertex.cpp Material.cpp Renderer.cpp ObjLoader.cpp ObjMaterial.cpp Renderable.cpp UBOUniform.cpp PMDL.cpp)

target_link_libraries (OpenGLBasic GL SDL2 SDL2_image)

