#include "SQLServer.h"

#include <QDebug>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <locale.h>
#include "Model.h"

#include <postgresql/libpq-fe.h>

SQLServer::SQLServer(Model *model)
    : model(model)
{
    setlocale(LC_NUMERIC, "C");

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

    // -- readSpec
    const char *readPointSpecQuery = "SELECT * FROM Point_spec LEFT JOIN Colors ON Colors.Color_ID = Point_spec.Color";
    PGresult *readPointSpec = PQprepare(connection, "readPointSpec", readPointSpecQuery, 0, NULL);
    checkResult(readPointSpec);

    const char *readLineSpecQuery = "SELECT * FROM Line_spec LEFT JOIN Colors ON Colors.Color_ID = Line_spec.Color";
    PGresult *readLineSpec = PQprepare(connection, "readLineSpec", readLineSpecQuery, 0, NULL);
    checkResult(readLineSpec);
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

        std::shared_ptr<PointSpec> spec;

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
            int spec_id = std::stoi(PQgetvalue(result, 0, 4));

            if (spec = model->getPointSpec(spec_id)) {
                point = std::make_shared<Point>(x, y, z, spec);
            } else {
                readSQLPointSpec();
                spec = model->getPointSpec(spec_id);
                point = std::make_shared<Point>(x, y, z, spec);
            }
        }
        
        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error: Point";
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

        std::shared_ptr<PointSpec> spec;

        int rows = PQntuples(result);
        int cols = PQnfields(result);

        for (int row = 0; row < rows; row++) {
            int id = std::stoi(PQgetvalue(result, row, 0));
            float x = std::stof(PQgetvalue(result, row, 1));
            float y = std::stof(PQgetvalue(result, row, 2));
            float z = std::stof(PQgetvalue(result, row, 3));
            int spec_id = std::stoi(PQgetvalue(result, row, 4));

            if (spec = model->getPointSpec(spec_id)) {
                points.push_back(std::make_shared<Point>(x, y, z, spec));
            } else {
                readSQLPointSpec();
                spec = model->getPointSpec(spec_id);
                points.push_back(std::make_shared<Point>(x, y, z, spec));
            }
        }

        for (const auto& point : points) {
            std::cout << "ID: " << point->getID() << "\t" << point->getX() << "\t" << point->getY() << "\t" << point->getZ() << std::endl;
        }

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error: Point";
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

            std::shared_ptr<LineSpec> spec;

            int rows = PQntuples(result);
            int cols = PQnfields(result);

            for (int row = 0; row < rows; row++) {
                int p1_ID = std::stoi(PQgetvalue(result, row, 1));
                int p2_ID = std::stoi(PQgetvalue(result, row, 2));
                std::shared_ptr<Point> p1 = readPointByID(p1_ID);
                std::shared_ptr<Point> p2 = readPointByID(p2_ID);
                int spec_id = std::stoi(PQgetvalue(result, row, 3));
                if (spec = model->getLineSpec(spec_id)) {
                    lines.push_back(std::make_shared<Line>(p1, p2, spec));
                } else {
                    readSQLLineSpec();
                    spec = model->getLineSpec(spec_id);
                    lines.push_back(std::make_shared<Line>(p1, p2, spec));
                }
            }

            for (const auto& line : lines) {
                std::cout << line->getP2()->getID() << "\t" << line->getP1()->getID() << std::endl;
            }
            
            PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error: Line";
    }
    return lines;
}

std::vector<std::shared_ptr<PointSpec>> SQLServer::readSQLPointSpec()
{
    std::vector<std::shared_ptr<PointSpec>> pointSpecs;
    try {
        qDebug() << "reading SQL Point Spec";

        PGresult *result = PQexecPrepared(connection, "readPointSpec", 0, NULL, NULL, NULL, 0);
        checkResult(result);

        int rows = PQntuples(result);
        int cols = PQnfields(result);

        for (int row = 0; row < rows; row++) {
            int id = std::stoi(PQgetvalue(result, row, 0));
            float size = std::stof(PQgetvalue(result, row, 2));
            std::string name = PQgetvalue(result, row, 3);
            float color[4];
            color[0] = std::stof(PQgetvalue(result, row, 5));
            color[1] = std::stof(PQgetvalue(result, row, 6));
            color[2] = std::stof(PQgetvalue(result, row, 7));
            color[3] = std::stof(PQgetvalue(result, row, 8));
            std::shared_ptr<PointSpec> spec = std::make_shared<PointSpec>(name, color, size, id);
            pointSpecs.push_back(spec);
        }

        for (const auto& spec : pointSpecs) {
            std::cout << spec->getName() << " " << spec->getColor() << " " << spec->getSize() << " " << spec->getID() << std::endl;
        }

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error: Point Spec";
    }
    return pointSpecs;
}

std::vector<std::shared_ptr<LineSpec>> SQLServer::readSQLLineSpec()
{
    std::vector<std::shared_ptr<LineSpec>> lineSpecs;
    try {
        qDebug() << "reading SQL Line Spec";

        PGresult *result = PQexecPrepared(connection, "readLineSpec", 0, NULL, NULL, NULL, 0);
        checkResult(result);

        int rows = PQntuples(result);
        int cols = PQnfields(result);

        for (int row = 0; row < rows; row++) {
            int id = std::stoi(PQgetvalue(result, row, 0));
            float width = std::stof(PQgetvalue(result, row, 2));
            std::string name = PQgetvalue(result, row, 3);
            float color[4];
            color[0] = std::stof(PQgetvalue(result, row, 5));
            color[1] = std::stof(PQgetvalue(result, row, 6));
            color[2] = std::stof(PQgetvalue(result, row, 7));
            color[3] = std::stof(PQgetvalue(result, row, 8));
            std::shared_ptr<LineSpec> spec = std::make_shared<LineSpec>(name, color, width, id);
            lineSpecs.push_back(spec);
        }

        for (const auto& spec : lineSpecs) {
            std::cout << spec->getName() << " " << spec->getColor() << " " << spec->getWidth() << " " << spec->getID() << std::endl;
        }

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL read Error: Line Spec";
    }
    return lineSpecs;
}




// -- write --

int SQLServer::newPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec)
{
    char xS[32], yS[32], zS[32], specS[32];

    snprintf(xS, sizeof(xS), "%.6g", x);
    snprintf(yS, sizeof(yS), "%.6g", y);
    snprintf(zS, sizeof(zS), "%.6g", z);
    snprintf(specS, sizeof(specS), "%d", spec->getID());

    const char *parVal[4] = {xS, yS, zS, specS};

    qDebug() << "Point: " << x << " " << y << " " << z << " " << spec->getID();


    try {
            qDebug() << "writing Point in SQL";

            PGresult *result = PQexecPrepared(connection, "insertPoint3D", 4, parVal, NULL, NULL, 0);
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

void SQLServer::newLine(int p1_ID, int p2_ID, std::shared_ptr<LineSpec> spec)
{
    try {
        qDebug() << "writing Line in SQL";

        char p1_ID_str[32], p2_ID_str[32], specS[32];
        snprintf(p1_ID_str, sizeof(p1_ID_str), "%d", p1_ID);
        snprintf(p2_ID_str, sizeof(p2_ID_str), "%d", p2_ID);
        snprintf(specS, sizeof(specS), "%d", spec->getID());
        
        const char *parVal[3] = {p1_ID_str, p2_ID_str, specS};
        PGresult *result = PQexecPrepared(connection, "insertLine", 3, parVal, NULL, NULL, 0);
        checkResult(result);

        PQclear(result);

    } catch (const std::exception &e) {
        qDebug() << "SQL write Error";
    }
}

