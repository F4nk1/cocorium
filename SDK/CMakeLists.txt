# Forzar C++17 para asegurar compatibilidad con características modernas de nlohmann/json y std::function
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crear la librería estática del SDK
add_library(CocoriumSDK STATIC src/Cocorium.cpp)

# 1. Cabeceras Públicas: Cualquier ejecutable (Launcher/Server) que enlace con CocoriumSDK 
# sabrá automáticamente dónde buscar "CocoriumSDK/Cocorium.h"
target_include_directories(CocoriumSDK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Cabeceras Privadas (Tu configuración original para dependencias)
target_include_directories(CocoriumSDK PRIVATE ${CMAKE_BINARY_DIR}/_deps/enet-src/include)
target_include_directories(CocoriumSDK PRIVATE ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include)

# 3. ENLACE BINARIO (¡Nuevo e importante!): 
# nlohmann_json es "header-only" (solo cabeceras), pero ENet necesita enlazarse.
# Asumimos que tu CMake principal construye un target llamado 'enet'
target_link_libraries(CocoriumSDK PRIVATE enet)