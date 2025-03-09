#ifndef SQLServer_H
#define SQLServer_H

#include "postgresql/libpq-fe.h"


class SQLServer {
    public:
        SQLServer();

        void writeSQL(const char *parVal[2]);
        void readSQL();

        int newPoint(float x, float y, float z);
        void newLine(int p1_ID, int p2_ID);

        ~SQLServer();


    private:
        PGconn *connection;

        void checkResult(PGresult *result);
};

#endif
