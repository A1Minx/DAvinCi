

class Object3D {
    public:

        Object3D();
        ~Object3D();
        void draw();

    private:
        int syncStatus;
        // syncStatus gets stored bitwise / Implement data exchange at parent level and set children locally
        // Exclusive - local data coherent with Database, no other read/write access
        // Shared - local Data coherent with Database, other user has read/write access
        // SharedReadOnly - local Data coherent with Database, readonly
        // InvalidReadOnly - local Data incoherent with Database because of Remote Change - read only access so cant be changed anyway
        // Invalid - local Data incoherent with Database because of Remote Change - Object cant be changed until synced
        // ExclusiveModified - local Data ahead of Database, Exclusive Write access
        // SharedModified - local Data ahead of Database, Shared write access (show loading until synced, set others Invalid)

};
