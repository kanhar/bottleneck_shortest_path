Input
1) Set of points and segment obstacles as input, as well as a start and target point,and shows the bottle neck shortest path between them.
2) The program takes one command line argument,a file name; 
3) There are less than 1000 points and obstacles

[-100,500]x[0,300] 	//co-ordinate
P (-19,273)
P (-16,30)
P (-48,50)
P (28,117)
S (20,100) (55,63)
S (5,10)   (15,37)

 the following lines give the set of points, the first two points are the start and the target point of the path.they have the format P (-19,273) after these come some lines with obstacles; they are line segments described by start and end points\, with format S (20,100) (55,63) you can parse these files with scanf(e.g., format "[%d,%d]x[%d,%d]" for the first line). there are less than 1000 points and obstacles. 
 
 1) Check for each pair of points and each segment whether the pair of points is seperated by the segment;
 2) If not compute the distance and put it in an array of occuring distances.
 3) Sort the array.
 4) Perform binary search on that array to find the smallest distance d such that if you connect only point pairs of distance at most d, the start to target point are in the same connected component.then you select a path from start to target in that component,and display the points,obstacles, and the resulting path in a window. 
