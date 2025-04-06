\connect DAvinCi;

----- Specs -----
CREATE TABLE Colors (
    Color_ID BIGSERIAL PRIMARY KEY,
    R DOUBLE PRECISION,
    G DOUBLE PRECISION,
    B DOUBLE PRECISION,
    A DOUBLE PRECISION
);

CREATE TABLE Point_spec (
    Spec_ID BIGSERIAL PRIMARY KEY,
    Color INTEGER REFERENCES Colors(Color_ID),
    Size DOUBLE PRECISION,
    Name VARCHAR(100)
);

CREATE TABLE Line_spec (
    Spec_ID BIGSERIAL PRIMARY KEY,
    Color INTEGER REFERENCES Colors(Color_ID),
    Width DOUBLE PRECISION,
    Name VARCHAR(100)
);


----- Basic Geometry -----
CREATE TABLE Points (
    Points_ID BIGSERIAL PRIMARY KEY,
    Points_x DOUBLE PRECISION,
    Points_y DOUBLE PRECISION,
    Points_z DOUBLE PRECISION,
    Spec INTEGER REFERENCES Point_spec(Spec_ID),
    Name VARCHAR(100)
);

CREATE TABLE Lines (
    Lines_ID BIGSERIAL PRIMARY KEY,
    LINES_PointA_Points_ID INTEGER REFERENCES Points(Points_ID),
    Lines_PointB_Points_ID INTEGER REFERENCES Points(Points_ID),
    Spec INTEGER REFERENCES Line_spec(Spec_ID),
    Lines_length DOUBLE PRECISION
);


----- Composed Geometry -----
CREATE TABLE composedObjects (
    composedObject_ID BIGSERIAL PRIMARY KEY,
    Name VARCHAR(100),
    Parent_ID INTEGER REFERENCES composedObjects(composedObject_ID)
);

CREATE TABLE Obj2Line (
    ID BIGSERIAL PRIMARY KEY,
    Object_ID INTEGER REFERENCES composedObjects(composedObject_ID),
    Line_ID INTEGER REFERENCES Lines(Lines_ID)
);

CREATE TABLE Obj2Point (
    ID BIGSERIAL PRIMARY KEY,
    Object_ID INTEGER REFERENCES composedObjects(composedObject_ID),
    Point_ID INTEGER REFERENCES Points(Points_ID)
);
