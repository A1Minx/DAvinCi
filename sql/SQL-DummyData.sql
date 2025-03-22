\connect DAvinCi;


INSERT INTO Colors (Color_ID,R,G,B,A) Values (1,1,0,0,1); --red
INSERT INTO Colors (Color_ID,R,G,B,A) Values (2,0,1,0,1); --green
INSERT INTO Colors (Color_ID,R,G,B,A) Values (3,0,0,1,1); --blue
INSERT INTO Colors (Color_ID,R,G,B,A) Values (4,1,1,0,1); --yellow
INSERT INTO Colors (Color_ID,R,G,B,A) Values (5,1,0,1,1); --purple
INSERT INTO Colors (Color_ID,R,G,B,A) Values (6,0.5,0.5,0.5,1); --grey
INSERT INTO Colors (Color_ID,R,G,B,A) Values (7,0,0,0,1); --black
INSERT INTO Colors (Color_ID,R,G,B,A) Values (8,1,1,1,1); --White

INSERT INTO Point_spec (Spec_ID, Color, Size, Name) Values (1, 1, 5, 'large red');
INSERT INTO Point_spec (Spec_ID, Color, Size, Name) Values (2, 2, 10, 'larger green');
INSERT INTO Point_spec (Spec_ID, Color, Size, Name) Values (3, 4, 10, 'larger yellow');

INSERT INTO Line_spec (Spec_ID, Color, Width, Name) Values (1, 1, 1.0, 'thin red');
INSERT INTO Line_spec (Spec_ID, Color, Width, Name) Values (2, 1, 5.0, 'thic red');
INSERT INTO Line_spec (Spec_ID, Color, Width, Name) Values (3, 6, 1.0, 'thin grey');
INSERT INTO Line_spec (Spec_ID, Color, Width, Name) Values (4, 6, 5.0, 'thic grey');
INSERT INTO Line_spec (Spec_ID, Color, Width, Name) Values (5, 2, 0.5, 'faded green');


INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (100, 100, 100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (100, 200, 100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (400, 100, 100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (400, 200, 100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (100, 100, -100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (400, 200, -100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (100, 200, -100, 1);
INSERT INTO Points (Points_x, Points_y, Points_z, Spec) Values (400, 100, -100, 1);

INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (1, 2, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (1, 3, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (3, 4, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (2, 4, 1);

INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (5, 7, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (5, 8, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (6, 8, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (7, 6, 1);

INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (1, 5, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (4, 6, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (2, 7, 1);
INSERT INTO Lines (LINES_PointA_Points_ID, Lines_PointB_Points_ID, Spec) Values (3, 8, 1);














