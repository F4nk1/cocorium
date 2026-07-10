#!/bin/bash

echo "Inicializando el cluster de base de datos..."
# Inicializa el cluster sólo si no ha sido inicializado antes
if [ ! -d "/var/lib/postgres/data/base" ]; then
    sudo -u postgres initdb -D /var/lib/postgres/data
else
    echo "El cluster ya está inicializado."
fi

echo "Iniciando servicio de PostgreSQL..."
sudo systemctl enable --now postgresql

echo "Esperando a que PostgreSQL inicie..."
sleep 3

echo "Creando usuario 'admin' y base de datos 'cocorium'..."
sudo -u postgres psql -c "CREATE USER admin WITH PASSWORD 'root';" 2>/dev/null || echo "El usuario ya existe."
sudo -u postgres psql -c "CREATE DATABASE cocorium OWNER admin;" 2>/dev/null || echo "La base de datos ya existe."

echo "Otorgando privilegios..."
sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE cocorium TO admin;"

echo "¡Configuración completada con éxito!"
