#ifndef SQLServer_H
#define SQLServer_H

#include "postgresql/libpq-fe.h"
#include "Point.h"
#include "Line.h"
#include <vector>
#include <memory>

class Model;

class SQLServer {
    public:
        SQLServer(Model *model);

        std::vector<std::shared_ptr<Line>> readSQLLines();
        std::vector<std::shared_ptr<Point>> readSQLPoints();
        std::vector<std::shared_ptr<PointSpec>> readSQLPointSpec();
        std::vector<std::shared_ptr<LineSpec>> readSQLLineSpec();

        std::shared_ptr<Point> readPointByID(int id);

        int newPoint(float x, float y, float z, std::shared_ptr<PointSpec> spec);
        void newLine(int p1_ID, int p2_ID, std::shared_ptr<LineSpec> spec);

        ~SQLServer();


    private:
        PGconn *connection;

        void checkResult(PGresult *result);

        Model *model;
};

#endif
