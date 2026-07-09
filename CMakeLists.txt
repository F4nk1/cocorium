cmake_minimum_required(VERSION 3.14)
project(cocorium VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadimos el motor Beyota como subdirectorio
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../beyota beyota_build)

# Add platform modules
add_subdirectory(SDK)
add_subdirectory(Launcher)
add_subdirectory(Server)
