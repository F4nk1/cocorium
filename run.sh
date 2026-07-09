#!/bin/bash

echo "Iniciando Ecosistema Cocorium..."

# 1. Matar instancias previas si las hay
killall CocoriumServer 2>/dev/null
killall CocoriumLauncher 2>/dev/null

# 2. Iniciar el Servidor C++ en segundo plano
echo "[+] Iniciando Cocorium Server en el puerto 7777..."
./build/Server/CocoriumServer &
SERVER_PID=$!

# Darle 1 segundo al servidor para iniciar
sleep 1

# 3. Iniciar el Launcher
echo "[+] Iniciando Cocorium Launcher..."
cd build/Launcher
./CocoriumLauncher

# 4. Al cerrar el launcher, matar el servidor
echo "Cerrando Launcher. Apagando el servidor..."
kill $SERVER_PID
echo "Ecosistema apagado correctamente."
