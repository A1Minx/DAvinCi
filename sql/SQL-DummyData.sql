\connect DAvinCi;

INSERT INTO Points (Points_x, Points_y, Points_z) Values (100, 100, 100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (100, 200, 100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (400, 100, 100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (400, 200, 100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (100, 100, -100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (400, 200, -100);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (100, 200, 0);
INSERT INTO Points (Points_x, Points_y, Points_z) Values (400, 100, 0);

INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID) Values (1, 2);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID) Values (3, 4);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID) Values (1, 3);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID) Values (2, 4);
