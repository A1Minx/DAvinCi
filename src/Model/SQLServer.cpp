#include "SQLServer.h"

#include <QDebug>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

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

    // -- readPointByID
    const char *readPointByIDQuery = "SELECT * FROM Points WHERE Points_ID = $1";
    PGresult *readPointByID = PQprepare(connection, "readPointByID", readPointByIDQuery, 1, NULL);
    checkResult(readPointByID);

    // -- readLines
    const char *readLinesQuery = "SELECT * FROM Lines";
    PGresult *readLines = PQprepare(connection, "readLines", readLinesQuery, 0, NULL);
    checkResult(readLines);
}

SQLServer::~SQLServer()
{
     PQfinish(connection);
     qDebug() << "SQL Server connection ended";
}


// -- Internal Functions --
void SQLServer::checkResult(PGresult *result)
{
    ExecStatusType status = PQresultStatus(result);
    if (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK) {
        return;
    } else {
        qDebug() << "SQL Error:" << PQresultErrorMessage(result);
        throw std::runtime_error("Prepare Error");
    }
}




// ----- Actions -----
// -- read --

std::shared_ptr<Point> SQLServer::readPointByID(int id)
{
    std::shared_ptr<Point> point;
    try {
        qDebug() << "reading SQL Points";

        char id_str[32];
        snprintf(id_str, sizeof(id_str), "%d", id);

        const char *parVal[1] = {id_str};

        PGresult *result = PQexecPrepared(connection, "readPointByID", 1, parVal, NULL, NULL, 0);
        checkResult(result);

        int rows = PQntuples(result);
        int cols = PQnfields(result);
        
        if (rows > 0) {
            float x = std::stof(PQgetvalue(result, 0, 1));
            float y = std::stof(PQgetvalue(result, 0, 2));
            float z = std::stof(PQgetvalue(result, 0, 3));
            point = std::make_shared<Point>(x, y, z, id);
        }
        
        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error";
    }
    return point;
}

std::vector<std::shared_ptr<Point>> SQLServer::readSQLPoints() 
{
    std::vector<std::shared_ptr<Point>> points;
    try {
        qDebug() << "reading SQL Points";

        PGresult *result = PQexecPrepared(connection, "readPoints", 0, NULL, NULL, NULL, 0);
        checkResult(result);

        int rows = PQntuples(result);
        int cols = PQnfields(result);

        for (int row = 0; row < rows; row++) {
            int id = std::stoi(PQgetvalue(result, row, 0));
            float x = std::stof(PQgetvalue(result, row, 1));
            float y = std::stof(PQgetvalue(result, row, 2));
            float z = std::stof(PQgetvalue(result, row, 3));
            points.push_back(std::make_shared<Point>(x, y, z, id));
        }

        for (const auto& point : points) {
            std::cout << "ID: " << point->getID() << "\t" << point->getX() << "\t" << point->getY() << "\t" << point->getZ() << std::endl;
        }

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error";
    }
    return points;
}

std::vector<std::shared_ptr<Line>> SQLServer::readSQLLines()
{
    std::vector<std::shared_ptr<Line>> lines;
    try {
            qDebug() << "reading SQL Lines";

            PGresult *result = PQexecPrepared(connection, "readLines", 0, NULL, NULL, NULL, 0);
            checkResult(result);

            int rows = PQntuples(result);
            int cols = PQnfields(result);

            for (int row = 0; row < rows; row++) {
                int p1_ID = std::stoi(PQgetvalue(result, row, 1));
                int p2_ID = std::stoi(PQgetvalue(result, row, 2));
                std::shared_ptr<Point> p1 = readPointByID(p1_ID);
                std::shared_ptr<Point> p2 = readPointByID(p2_ID);
                lines.push_back(std::make_shared<Line>(p1, p2));
            }

            for (const auto& line : lines) {
                std::cout << line->getP2()->getID() << "\t" << line->getP1()->getID() << std::endl;
            }
            
            PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error";
    }
    return lines;
}


// -- write --

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
            qDebug() << e.what();
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

