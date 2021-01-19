bottleneck_shortest_path
========================

```
Input
1) Set of points and segment obstacles as input, as well as a start and target point
2) The program takes one command line argument,a file name; 
3) There are less than 1000 points and obstacles

Output
1) Result: Shows the bottle neck shortest path between them.

[-100,500]x[0,300] 	//co-ordinates for Boundary
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
```

Wikipedia Link: http://en.wikipedia.org/wiki/Widest_path_problem

Solution
========================

```
Usage: 
g++ main.cpp -lX11 -lm -L/usr/X11R6/lib
./a.out brass.tst

Input:
brass.tst (supplied via email)

Output:
output.pdf (contains a visual with and without obstacle detection)

Time: 
< 3 seconds on standard 1.6 GHZ 2 GB RAM machine.

Known Errors:
Xlib Code cannot draw negative Points

G++ Version:
$ g++ -v:Configured with: ../src/configure -v --with-pkgversion='Ubuntu 4.4.3-4ubuntu5' --with-bugurl=file:///usr/share/doc/gcc-4.4/README.Bugs --enable-languages=c,c++,fortran,objc,obj-c++ --prefix=/usr --enable-shared --enable-multiarch --enable-linker-build-id --with-system-zlib --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.4 --program-suffix=-4.4 --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-plugin --enable-objc-gc --enable-targets=all --disable-werror --with-arch-32=i486 --with-tune=generic --enable-checking=release --build=i486-linux-gnu --host=i486-linux-gnu --target=i486-linux-gnu
Thread model: posix

Using built-in specs.
Target: i486-linux-gnu
```

