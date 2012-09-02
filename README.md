bottleneck_shortest_path
========================

In graph algorithms, the widest path problem, also known as the bottleneck shortest path problem or the maximum capacity path problem, is the problem of finding a path between two designated vertices in a weighted directed graph, maximizing the weight of the minimum-weight edge in the path. For instance, if the graph represents connections between routers in the Internet, and the weight of an edge represents the bandwidth of a connection between two routers, the widest path problem is the problem of finding an end-to-end path between two Internet nodes that has the maximum possible bandwidth.[1] The weight of the minimum-weight edge is known as the capacity or bandwidth of the path. As well as its applications in network routing, the widest path problem is also an important component of the Schulze method for deciding the winner of a multiway election,[2] and has been applied to digital compositing,[3] metabolic analysis,[4] and the computation of maximum flows.[5] It is possible to adapt most shortest path algorithms to compute widest paths, by modifying them to use the bottleneck distance instead of path length.[6] However, in many cases even faster algorithms are possible.

Input
1) Set of points and segment obstacles as input, as well as a start and target point
2) The program takes one command line argument,a file name; 
3) There are less than 1000 points and obstacles

Output
1) Result: Shows the bottle neck shortest path between them.

[-100,500]x[0,300]   //co-ordinates for Boundary
P (-19,273)
P (-16,30)
P (-48,50)
P (28,117)
S (20,100) (55,63)
S (5,10)   (15,37)

The following lines give the set of points, the first two points are the start and the target point of the path.
They have the format P (-19,273) after these come some lines with obstacles; they are line segments described by start 
and end points, with format S (20,100) (55,63) you can parse these files with scanf(e.g., format "[%d,%d]x[%d,%d]" 
for the first line). there are less than 1000 points and obstacles. 
 
 1) Check for each pair of points and each segment whether the pair of points is seperated by the segment;
 2) If not compute the distance and put it in an array of occuring distances.
 3) Sort the array.
 4) Perform binary search on that array to find the smallest distance d such that if you connect only point pairs 
of distance at most d, the start to target point are in the same connected component.then you select a path 
from start to target in that component,and display the points,obstacles, and the resulting path in a window. 