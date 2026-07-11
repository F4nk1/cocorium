set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
# ... (tu configuración actual de C++17 y project) ...

include(FetchContent)

# 1. Descargar la librería Bcrypt para C++
FetchContent_Declare(
    bcrypt_cpp
    GIT_REPOSITORY https://github.com/trusch/libbcrypt.git
    GIT_TAG        master
)
cmake_policy(PUSH)
if(POLICY CMP0218)
    cmake_policy(SET CMP0218 OLD)
endif()
set(CMAKE_WARN_DEPRECATED OFF)
FetchContent_MakeAvailable(bcrypt_cpp)
cmake_policy(POP)

# ... (tus otras dependencias como enet y nlohmann_json) ...

add_executable(CocoriumServer src/main.cpp src/PostgreSQLDatabase.cpp src/ServerConfigManager.cpp ../Common/src/Logger.cpp)

# 2. Enlazar bcrypt al target del servidor (añadimos 'bcrypt' a la lista)
target_link_libraries(CocoriumServer PRIVATE enet pqxx pq bcrypt)

# Mantienes tus includes privados
target_include_directories(CocoriumServer PRIVATE ${CMAKE_BINARY_DIR}/_deps/enet-src/include)
target_include_directories(CocoriumServer PRIVATE ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include)
target_include_directories(CocoriumServer PRIVATE ${CMAKE_BINARY_DIR}/_deps/bcrypt_cpp-src/include)
target_include_directories(CocoriumServer PRIVATE ../Common/include)