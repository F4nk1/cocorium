#!/bin/bash

# Elimina por completo la base de datos y el usuario admin.

echo "Eliminando base de datos cocorium..."
sudo -u postgres psql -c "DROP DATABASE IF EXISTS cocorium;"

echo "Eliminando usuario admin..."
sudo -u postgres psql -c "DROP USER IF EXISTS admin;"

echo "Limpieza completada. Si quieres detener PostgreSQL, puedes ejecutar: sudo systemctl stop postgresql"
