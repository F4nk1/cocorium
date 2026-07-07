cmake_minimum_required(VERSION 3.14)
project(cocorium VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos el motor Beyota como subdirectorio
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../beyota beyota_build)

set(SOURCES
    src/main.cpp
    src/CocoriumLauncher.cpp
)

add_executable(CocoriumLauncher ${SOURCES})

# Incluimos los headers de cocorium
target_include_directories(CocoriumLauncher PRIVATE src)

# Enlazamos con la librería dinámica de Beyota
target_link_libraries(CocoriumLauncher PRIVATE beyota)
