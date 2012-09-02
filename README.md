bottleneck_shortest_path
========================

In graph algorithms, the widest path problem, also known as the bottleneck shortest path problem or the maximum capacity path problem, is the problem of finding a path between two designated vertices in a weighted directed graph, maximizing the weight of the minimum-weight edge in the path. For instance, if the graph represents connections between routers in the Internet, and the weight of an edge represents the bandwidth of a connection between two routers, the widest path problem is the problem of finding an end-to-end path between two Internet nodes that has the maximum possible bandwidth.[1] The weight of the minimum-weight edge is known as the capacity or bandwidth of the path. As well as its applications in network routing, the widest path problem is also an important component of the Schulze method for deciding the winner of a multiway election,[2] and has been applied to digital compositing,[3] metabolic analysis,[4] and the computation of maximum flows.[5] It is possible to adapt most shortest path algorithms to compute widest paths, by modifying them to use the bottleneck distance instead of path length.[6] However, in many cases even faster algorithms are possible.

http://en.wikipedia.org/wiki/Widest_path_problem

More details: http://kmwiki.info/wiki/index.php/Code/CPlusPlus/BottleneckShortestPath