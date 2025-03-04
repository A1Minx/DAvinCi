#include "SQLServer.h"

#include <QDebug>
#include <iostream>

#include <postgresql/libpq-fe.h>

SQLServer::SQLServer()
{

    connection = PQconnectdb("");

    if (PQstatus(connection) != CONNECTION_OK) {

            PQfinish(connection);
            throw std::runtime_error("Database Connection not successful!");
        }
    qDebug() << "init SQL Server Connection";

    // -- Prepared Statements
    // -- InsertPoint
    const char *InsertPointQuery = "INSERT INTO Points (Points_x, Points_y) Values ($1, $2)";
    PGresult *insertPoint = PQprepare(connection, "insertPoint", InsertPointQuery, 2, NULL);
    checkResult(insertPoint);

    // -- readPoints
    const char *readPointsQuery = "SELECT * FROM Points";
    PGresult *readPoints = PQprepare(connection, "readPoints", readPointsQuery, 0, NULL);
    checkResult(readPoints);
}

SQLServer::~SQLServer()
{
     PQfinish(connection);
     qDebug() << "SQL Server connection ended";
}


// -- Internal Functions --
void SQLServer::checkResult(PGresult *result)
{
    if (PQresultStatus(result) == PGRES_COMMAND_OK) {
            return;
        }
    else if (PQresultStatus(result) == PGRES_TUPLES_OK){
            return;
    } else {
            throw std::runtime_error("Prepare Error");
        }
}




// -- Actions --
void SQLServer::readSQL()
{
    try {
        qDebug() << "reading SQL";

        PGresult *result = PQexecPrepared(connection, "readPoints", 0, NULL, NULL, NULL, 0);
        checkResult(result);

        int rows = PQntuples(result);
        int cols = PQnfields(result);

        for (int col = 0; col < cols; col++) {
            std::cout << PQfname(result, col);
            if (col < cols - 1)
                std::cout << "\t";
        }
        std::cout << std::endl;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                std::cout << PQgetvalue(result, row, col);
                if (col < cols - 1)
                    std::cout << "\t";
            }
            std::cout << std::endl;
        }

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error";
    }
}

void SQLServer::writeSQL(const char *parVal[2])
{

    try {
            qDebug() << "writing SQL";

            PGresult *result = PQexecPrepared(connection, "insertPoint", 2, parVal, NULL, NULL, 0);
            checkResult(result);

            PQclear(result);

        } catch (const std::exception &e) {
            qDebug() << "SQL write Error";
        }
}



