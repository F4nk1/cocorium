add_executable(CocoriumServer src/main.cpp src/DatabaseManager.cpp)

# Enlazamos con enet que está disponible gracias a beyota_build y con sqlite3
target_link_libraries(CocoriumServer PRIVATE enet sqlite3)
target_include_directories(CocoriumServer PRIVATE ${CMAKE_BINARY_DIR}/_deps/enet-src/include)
target_include_directories(CocoriumServer PRIVATE ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include)
