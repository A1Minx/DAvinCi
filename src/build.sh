#!/bin/bash


source ../.Userdata.env

echo "Executing table creation script..."




cd ../sql

if [[ $1 != "-dummy" ]] then
    echo "build empty server"
    ./SQL-Setup.sh
fi
if [[ $1 == "-dummy" ]] then
    echo "build server with dummy data"
    ./SQL-Setup.sh -dummy
fi

echo "Compiling..."
cd ../src/build

if [[ $1 == "-clean" || $2 == "-clean" ]]; then
    echo "compile clean Build"
    rm ./*
fi

qmake ../qt.pro

make


echo "Setup completed successfully!"


echo "Starting Program"

./DAvinCi
