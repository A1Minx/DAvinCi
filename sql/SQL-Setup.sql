\connect DAvinCi;

CREATE TABLE Points (
    Points_ID BIGSERIAL PRIMARY KEY,
    Points_x DOUBLE PRECISION,
    Points_y DOUBLE PRECISION,
    Points_z DOUBLE PRECISION,
    Name VARCHAR(100)
);

CREATE TABLE Lines (
    Lines_ID BIGSERIAL PRIMARY KEY,
    LINES_PointA_Points_ID INTEGER REFERENCES Points (Points_ID),
    Lines_PointB_Points_ID INTEGER REFERENCES Points(Points_ID),
    Lines_length DOUBLE PRECISION
);
