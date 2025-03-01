#!/bin/bash

if [ -z "$DB_PASSWORD" ]; then
    echo "Error: DB_PASSWORD environment variable is not set."
    exit 1
fi

export PGPASSWORD="$DB_PASSWORD"


sudo -u postgres psql -d postgres -c "DROP DATABASE IF EXISTS \"$DB_NAME\";"


## Create DB ##
echo "Creating database '$DB_NAME'..."
createdb -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" "$DB_NAME"
if [[ $? -ne 0 ]]; then
    echo "Error: Failed to create database '$DB_NAME'."
    exit 1
fi
echo "Database '$DB_NAME' created successfully."

## Execute SQL script ##
echo "Executing SQL script 'SQL-Setup.sql'..."
psql -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" -d "$DB_NAME" -f ./SQL-Setup.sql
if [[ $? -ne 0 ]]; then
    echo "Error: Failed to execute SQL script 'SQL-Setup.sql'."
    exit 1
fi
echo "SQL script executed successfully."


echo "Database setup completed successfully."

unset PGPASSWORD
