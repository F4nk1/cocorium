# Cocorium Platform

Cocorium is the official game launcher and ecosystem platform for games built on top of the **Beyota Engine**. 
It features a borderless, scalable console-like interface designed to manage, display, and launch installed games natively.

## Features
- **Adaptive Fullscreen**: Borderless execution capturing the full viewport.
- **Dynamic Execution**: Safely isolates system resources by suspending itself before launching games.
- **Console-like UI**: Nintendo Switch / eShop inspired UI layout using `ImGui`.

## Building
Cocorium relies on the Beyota Engine shared library.

```bash
mkdir build
cd build
cmake ..
make -j4
./CocoriumLauncher
```

## Dependencies
- C++17
- Beyota Engine (libbeyota.so)
- SDL2, OpenGL 3.3
