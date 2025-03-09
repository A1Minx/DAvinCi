#include "SQLServer.h"

#include <QDebug>
#include <iostream>
#include <cstdio>

#include <postgresql/libpq-fe.h>

SQLServer::SQLServer()
{
    connection = PQconnectdb("");
    if (!connection || PQstatus(connection) != CONNECTION_OK) {
        if (connection) {
            PQfinish(connection);
        }
        throw std::runtime_error("Database Connection not successful!");
    }
    qDebug() << "init SQL Server Connection";

    // -- Prepared Statements
    // -- InsertPoint
    //2D TODO: Delete
    const char *InsertPointQuery = "INSERT INTO Points (Points_x, Points_y) Values ($1, $2)";
    PGresult *insertPoint = PQprepare(connection, "insertPointTest", InsertPointQuery, 2, NULL);
    checkResult(insertPoint);

    //3D
    const char *InsertPoint3DQuery = "INSERT INTO Points (Points_x, Points_y, Points_z) Values ($1, $2, $3)";
    PGresult *insertPoint3D = PQprepare(connection, "insertPoint3D", InsertPoint3DQuery, 3, NULL);
    checkResult(insertPoint3D);

    // -- insertLine
    const char *InsertLineQuery = "INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID) Values ($1, $2)";
    PGresult *insertLine = PQprepare(connection, "insertLine", InsertLineQuery, 2, NULL);
    checkResult(insertLine);

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

            PGresult *result = PQexecPrepared(connection, "insertPointTest", 2, parVal, NULL, NULL, 0);
            checkResult(result);

            PQclear(result);

        } catch (const std::exception &e) {
            qDebug() << "SQL write Error";
        }
}


int SQLServer::newPoint(float x, float y, float z)
{
    char xS[32], yS[32], zS[32];

    snprintf(xS, sizeof(xS), "%.6g", x);
    snprintf(yS, sizeof(yS), "%.6g", y);
    snprintf(zS, sizeof(zS), "%.6g", z);

    const char *parVal[3] = {xS, yS, zS};
    try {
            qDebug() << "writing Point in SQL";

            PGresult *result = PQexecPrepared(connection, "insertPoint3D", 3, parVal, NULL, NULL, 0);
            checkResult(result);

            PQclear(result);

        } catch (const std::exception &e) {
            qDebug() << "SQL write Error";
        }

        PGresult *idResult = PQexec(connection, "SELECT lastval()");
        int newID = atoi(PQgetvalue(idResult, 0, 0));
        PQclear(idResult);
       
        return newID;
}

void SQLServer::newLine(int p1_ID, int p2_ID)
{
    try {
        qDebug() << "writing Line in SQL";

        char p1_ID_str[32], p2_ID_str[32];
        snprintf(p1_ID_str, sizeof(p1_ID_str), "%d", p1_ID);
        snprintf(p2_ID_str, sizeof(p2_ID_str), "%d", p2_ID);
        
        const char *parVal[2] = {p1_ID_str, p2_ID_str};
        PGresult *result = PQexecPrepared(connection, "insertLine", 2, parVal, NULL, NULL, 0);
        checkResult(result);

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL write Error";
    }
}

