#ifndef SQLServer_H
#define SQLServer_H

#include "postgresql/libpq-fe.h"
#include "Point.h"
#include "Line.h"
#include <vector>
#include <memory>
class SQLServer {
    public:
        SQLServer();

        std::vector<std::shared_ptr<Line>> readSQLLines();
        std::vector<std::shared_ptr<Point>> readSQLPoints();

        std::shared_ptr<Point> readPointByID(int id);

        int newPoint(float x, float y, float z);
        void newLine(int p1_ID, int p2_ID);

        ~SQLServer();


    private:
        PGconn *connection;

        void checkResult(PGresult *result);
};

#endif
