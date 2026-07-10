set(SOURCES
    src/main.cpp
    src/CocoriumLauncher.cpp
    src/GameLibraryManager.cpp
    src/UIManager.cpp
    src/ConfigManager.cpp
    src/ThemeManager.cpp
    src/SectionManager.cpp
    src/sections/LibrarySection.cpp
    src/sections/SettingsSection.cpp
    src/sections/HomeSection.cpp
)

add_executable(CocoriumLauncher ${SOURCES})

target_include_directories(CocoriumLauncher PRIVATE src)
target_include_directories(CocoriumLauncher PRIVATE ../SDK/include)
target_include_directories(CocoriumLauncher PRIVATE ${CMAKE_BINARY_DIR}/_deps/enet-src/include)
target_link_libraries(CocoriumLauncher PRIVATE beyota CocoriumSDK)
