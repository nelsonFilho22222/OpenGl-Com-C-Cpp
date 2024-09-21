cmake_minimum_required(VERSION 3.29)
project(untitled)

# Versão
set(CMAKE_CXX_STANDARD 20)

# Encontre a biblioteca SDL2
find_package(SDL2 REQUIRED)

# Encontre a biblioteca OpenGL
find_package(OpenGL REQUIRED)

# Inclua os diretórios da SDL2
include_directories(${SDL2_INCLUDE_DIRS})

# Executável
add_executable(untitled main.cpp)

# Vincule as bibliotecas SDL2 e OpenGL ao seu projeto
target_link_libraries(untitled ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
