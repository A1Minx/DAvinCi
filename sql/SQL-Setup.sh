#!/bin/bash

if [ -z "$PGPASSWORD" ]; then
    echo "Error: DB_PASSWORD environment variable is not set."
    exit 1
fi

export PGPASSWORD="$PGPASSWORD"


sudo -u postgres psql -d postgres -c "DROP DATABASE IF EXISTS \"$PGDATABASE\";"


## Create DB ##
echo "Creating database '$PGDATABASE'..."
createdb -h "$PGHOST" -p "$PGPORT" -U "$PGUSER" "$PGDATABASE"
if [[ $? -ne 0 ]]; then
    echo "Error: Failed to create database '$PGDATABASE'."
    exit 1
fi
echo "Database '$PGDATABASE' created successfully."

## Execute SQL script ##
echo "Executing SQL script 'SQL-Setup.sql'..."
psql -h "$PGHOST" -p "$PGPORT" -U "$PGUSER" -d "$PGDATABASE" -f ./SQL-Setup.sql
if [[ $? -ne 0 ]]; then
    echo "Error: Failed to execute SQL script 'SQL-Setup.sql'."
    exit 1
fi
echo "SQL script executed successfully."

if [[ $1 == "-dummy" ]]; then
    echo "adding dummy data"
    psql -h "$PGHOST" -p "$PGPORT" -U "$PGUSER" -d "$PGDATABASE" -f ./SQL-DummyData.sql
fi


echo "Database setup completed successfully."

unset PGPASSWORD
