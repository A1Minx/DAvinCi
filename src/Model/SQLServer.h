#ifndef SQLServer_H
#define SQLServer_H

#include "postgresql/libpq-fe.h"


class SQLServer {
    public:
        SQLServer();

        void writeSQL(const char *parVal[2]);
        void readSQL();

        void newPoint(float x, float y, float z);

        ~SQLServer();


    private:
        PGconn *connection;

        void checkResult(PGresult *result);
};

#endif
