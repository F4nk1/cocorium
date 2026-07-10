#!/bin/bash

# Reinicia la base de datos, eliminando todos los datos actuales.

echo "Eliminando la base de datos cocorium..."
sudo -u postgres psql -c "DROP DATABASE IF EXISTS cocorium;"

echo "Creando base de datos cocorium nuevamente..."
sudo -u postgres psql -c "CREATE DATABASE cocorium OWNER admin;"
sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE cocorium TO admin;"

echo "Base de datos reseteada con éxito."
