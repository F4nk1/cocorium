# Cocorium Platform

Cocorium is the official ecosystem platform for games built on top of the **Beyota Engine**.
It acts as a complete platform consisting of a borderless console-like Launcher, a secure backend Server, and a unified SDK.

## Architecture
The platform is split into three main modules:
- **Cocorium Launcher**: A Nintendo Switch / eShop inspired UI layout using `ImGui`. Designed to manage, display, and launch installed games natively.
- **Cocorium Server**: A high-performance UDP backend server written in C++ using `ENet`. It handles secure authentication (with `BCrypt`), sessions, and matchmaking.
- **Cocorium SDK**: An Object-Oriented networking library injected into the Launcher and available for game developers to easily integrate with the Server.

## Dependencies (Arch Linux)
The backend now utilizes **PostgreSQL** for a robust, production-ready environment, replacing the old SQLite system.

Before building the project, you must set up the local database. We provide a convenient script for Arch Linux environments:

```bash
# Installs PostgreSQL, libpqxx, initializes the cluster and sets up the database.
chmod +x scripts/*.sh
./scripts/setup_db.sh
```

## Building
Cocorium relies on the Beyota Engine shared library and modern CMake (3.10+).

```bash
mkdir build
cd build
cmake ..
make -j4
```

## Running the Ecosystem
To test the platform, you must run both the Server and the Launcher. A convenient `run.sh` script is provided in the root directory to handle this automatically:

```bash
# Starts the server in the background and opens the Launcher
./run.sh
```

## Administrative Scripts
Inside the `scripts/` folder you will find:
- `setup_db.sh`: Installs and configures the PostgreSQL environment.
- `reset_db.sh`: Drops and recreates the database cleanly (useful for resetting test data).
- `drop_db.sh`: Uninstalls the database from the system entirely.
