#!/bin/bash

source ../.Userdata.env

echo "Executing table creation script..."
cd ../sql
./SQL-Setup.sh

echo "Compiling..."
cd ../src/build

qmake ../qt.pro

make


echo "Setup completed successfully!"
