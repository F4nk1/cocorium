add_library(CocoriumSDK STATIC src/Cocorium.cpp)

target_include_directories(CocoriumSDK PUBLIC include)
target_include_directories(CocoriumSDK PRIVATE ${CMAKE_BINARY_DIR}/_deps/enet-src/include)
target_include_directories(CocoriumSDK PRIVATE ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include)
