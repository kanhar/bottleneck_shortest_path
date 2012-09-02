#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <stack>
#include <string>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/***************************************************************/
/**************************Constants****************************/
/***************************************************************/

const int MAX_POINTS        =   1024;
const bool OBSTACLE         =   true;
const bool VERBOSE          =   true;
const bool EXTRA_VERBOSE    =   false;

char *win_name_string       =   (char*)"Bottleneck Shortest Path";
char *icon_name_string      =   (char*)"Icon for Example Window";

/***************************************************************/
/**************************Classes******************************/
/***************************************************************/

/***************************************************************/
// Class Used a data structure to hold a Cartesian Point (x,y)
/***************************************************************/
class Point {
public:
   int x;
   int y;
   int name;

   Point(){
   }

   Point(const Point& p1)
   {
       x =      p1.x;
       y =      p1.y;
       name =   p1.name;
   }
      
   bool Equals(Point p)
   {
       return p.name == name? true: false;
   }
      
   string toString(int num)
   {
        std::stringstream ss;
        ss << num;
        return ss.str();
   }   

   void Print()
   {
       cout << "P" << name;
   }

   void PrintFull()
   {
       cout << "P" << name << '('<< x << ',' << y << ')' ;
   }

   string GetFull()
   {
       string tmp= "";
       tmp+= "P";
       tmp+= toString(name);
       tmp+= "(";
       tmp+= toString(x);
       tmp+= ",";
       tmp+= toString(y);
       tmp +=")";

       return tmp;
   }

};




/***************************************************************/
// Class Used to represent an Edge(or Line) between two Points**
/***************************************************************/

class Edge  {
public:
    Point start;
    Point end;
    float dist;
    int name;

    string toString(float num)
    {
         std::stringstream ss;
         ss << num;
         return ss.str();
    }

    Edge()
    {
        start = Point();
        end = Point();
        dist = 0.0;
        name = -1;
    }

    Edge(Edge& l1)
    {
        start = Point(l1.start);
        end = Point(l1.end);
        dist = l1.dist;
        name = l1.name;
    }

    float CalculateDistance(Point a, Point b){
        return sqrt (pow(b.y - a.y, 2) + pow(b.x - a.x,2) );
    }

    void CalculateDistance()
    {
        dist = CalculateDistance(start,end);
    }

    void PrintLine()
    {
        start.PrintFull();
        cout << "-";
        end.PrintFull();
        cout << '\t' << '\t' << "DIST: " << dist << endl;
    }

    string GetLine()
    {
        string tmp = "";
        tmp+= start.GetFull();
        tmp+= "-";
        tmp+= end.GetFull();
        tmp+= "\t\t DIST: ";
        tmp+= toString(dist);
        tmp += "\n";
        return tmp;
    }


};


/*****************************************************************/
// Class for a list of Points that cannot be repeated
// Serves also as a custom Implementation of a Stack <Set <Points>>
/*****************************************************************/
class Path  {
public:
   Point* points;
   int count;

   Path()
   {
       points = new Point[MAX_POINTS];
       count = 0;
   }

   Path(int n)
   {
       points = new Point[n];
   }

   bool PointExists(Point p)
   {
       for(int i=0;i<count; i++)
           if (points[i].Equals(p))
               return true;
       return false;
   }

   void AddPoint(Point p)
   {
        if( !PointExists(p))
            points[count++] = Point(p);
   }

   void Print()
   {
       string tmp = "";
       for(int i=0;i<count; i++)
       {
           if (i+1 == count)
           {
               points[i].PrintFull();
               break;
           }

           Edge l;
           l.start = Point(points[i]);
           l.end   = Point(points[i+1]);
           l.name  = i;
           l.CalculateDistance();
           l.PrintLine();
       }
   }

   string GetPath()
   {
       string tmp = "";
       for(int i=0;i<count; i++)
       {
           if (i+1 == count)
           {
               tmp+= points[i].GetFull();
               break;
           }

           Edge l;
           l.start = Point(points[i]);
           l.end   = Point(points[i+1]);
           l.name  = i;
           l.CalculateDistance();
           tmp+= l.GetLine();
       }
       return tmp;
   }

   Point GetHeadPoint()
   {
        return Point(points[count-1]);
   }

   void DeleteAll()
   {
       count = 0;
   }
};

/***************************************************************/
/****************Core Code *************************************/
/***************************************************************/
void Test();
void Parse();
void CalculateMinDistance();
void ReadFile();
void PrintDistances();
void PrintFile();
bool PathExistLEQDistance(float pLessThanDistance);

bool ClockWise(Point a, Point b, Point c);
bool IsObstructed(Point a, Point b, Point c, Point d);
bool IsObstructed(Point a, Point b);
float CalculateDistance(Point a, Point b);

int p_ptr, l_ptr, d_ptr, rd_ptr, c1, c2,c3,c4;
char *fileName;
Point *points, start, target;
Edge *obstacles, *edgeDistances, *rawdistances;
Path lastPath;

/***************************************************************/
/****************Graphics Xlib Code******************************/
/***************************************************************/
Display *display_ptr;
Screen *screen_ptr;
int screen_num;
char *display_name = NULL;
int display_width, display_height;

Window win;
int border_width;
int win_width, win_height;
int win_x, win_y;

XWMHints *wm_hints;
XClassHint *class_hints;
XSizeHints *size_hints;
XTextProperty win_name, icon_name;

XEvent report;

GC gc, gc_yellow, gc_red, gc_grey;
unsigned long valuemask = 0;
XGCValues gc_values, gc_yellow_values, gc_red_values, gc_grey_values;
Colormap color_map;
XColor tmp_color1, tmp_color2;

void Draw(int argc, char **argv);
void InitXLib(int argc, char **argv);
void DrawLine(GC pGC, int a, int b, int c, int d);
void DrawPoint(GC pGC, int a, int b);

int main(int argc, char **argv)
{
    if ( argc != 2 )
    {
        cout << "Incorrect Usage\n";
        cout << "Usage: g++ main.cpp brass.tst -lX11 -lm -L/usr/X11R6/lib " << endl;
        return -1;
    }

    ifstream the_file ( argv[1] );
    if ( !the_file.is_open() )
    {
        cout<<"Could not open file " << argv[1] << "\n";
        return -1;
    }

    fileName = argv[1];

    //Open Window with Obstacle Detection    
    Parse();
    CalculateMinDistance();

    InitXLib(argc, argv);
    Draw(argc, argv);

    return 0;
}


//This class performs a binary search for the Edge Distances Table
// attempting to find a distance d for which no Path exists.
// When such a distance is found and if the binary search is completed
//  the function will refer to the last valid Path and path distance (d) it found
//  and return that as the minimum distance.
void CalculateMinDistance()
{
    int first, last, mid;
    float midDist, lastDist;

    first       =   0;
    last        =   d_ptr -1;
    midDist     =   0.0;
    lastDist    =   0.0;

    while(first <= last)
    {
        mid     = (first+last)/2;
        midDist = edgeDistances[mid].dist;

        if (EXTRA_VERBOSE)
            cout << "\nFirst, Last, Mid, dist " << '\t' << first << " , " << last << " , " << mid << " , " << midDist <<endl;

        if (PathExistLEQDistance( midDist))
        {
            lastDist    = midDist;
            first       = mid + 1;

            if (EXTRA_VERBOSE)
            {
                cout << "Last Path:- \n";
                lastPath.Print();
            }
        }
        else
        {
            last =  mid - 1;

            if (EXTRA_VERBOSE)
            {
                cout << "No Path for Distance = " << midDist << endl;
            }
        }
    }

    cout << "Total Points = " << p_ptr << " and Total Connections = " << d_ptr <<endl;
    cout << "Note: Total Connections are twice what they should be. Refer to Function Parse(), Section: CalculateDistances " <<endl;
    cout << "\nPath from "; start.PrintFull() ; cout << " to "; target.PrintFull();
    cout << " must have distance at most d, where d = " << lastDist << " and that Path is :- \n";

    lastPath.Print();
}

//Core Function of Algorithm:
// This is not a recursive function, although it is a more efficient iterative version of a recursive function
// This function seeds a Path with the start point
// Then it adds this path to the stack
// Then for each element in the stack, it pops the head path, and checks to see if the path can be connected to other
//      vertices that have not already been connected to it.
// If it can then it adds this vertice to the path, and pushes the path back to the stk, and resumes the loop
// It ensures that all the vertices it is comparing the head path vertice to have a distance < pLessThanDistance
bool PathExistLEQDistance(float pLessThanDistance)
{
    int i;

    Path seedPoint, curPointList;
    Edge curLine;
    Point curStartPoint;
    stack<Path> stkPointList;

    //Seed Path contains start Point
    seedPoint.AddPoint(start);

    //StkPath contains a list of Possible Paths from start to Target
    stkPointList.push(seedPoint);

    while(!stkPointList.empty())
    {
        curPointList     = stkPointList.top();
        curStartPoint    = curPointList.GetHeadPoint();

        stkPointList.pop();

        for(i= 0;i< d_ptr; i++)
        {
            curLine         = edgeDistances[i];

            if (curLine.dist > pLessThanDistance)
                continue;

            if (curLine.start.Equals(curStartPoint))
            {
                Path tmp = curPointList;

                if (!curPointList.PointExists(curLine.end))
                {
                    tmp.AddPoint(curLine.end);
                    stkPointList.push(tmp);                    
                }
                if (curLine.end.Equals(target))
                {
                    lastPath = tmp;
                    return true;
                }
            }
        } 
    }

    return false;
}


//This function does the following :-
//1) reads in the file's points, obstacles and boundaries into local variables
//2) calculates the edge distances (with or without obstacle detection)
//3) Prints out the file as read into the local data structures (if Verbose option is set)
void Parse()
{
    FILE *input;
    int i, j, ret;

    p_ptr=0;
    l_ptr=0;

    obstacles = new Edge[100];
    points = new Point[100];
    
    //********************************************************************//
    //*******************BEGIN FILE READ**********************************//
    //********************************************************************//
    input= fopen(fileName, "r+");

    if (input==NULL)
        perror ("Error opening file");

    ret = fscanf(input, "[%d,%d]x[%d,%d]\n", &c1, &c2, &c3, &c4);

    while(feof(input)== 0)
    {
        ret = fscanf(input,"P (%d,%d)\n",&points[p_ptr].x,&points[p_ptr].y);
        if (ret == 0)
            break;
        points[p_ptr].name = p_ptr;
        p_ptr++;
    }

    start = points[0];
    target = points[1];

    while(feof(input)== 0)
    {
        ret = fscanf(input,"S (%d,%d) (%d,%d)\n",
                     &obstacles[l_ptr].start.x,
                     &obstacles[l_ptr].start.y,
                     &obstacles[l_ptr].end.x,
                     &obstacles[l_ptr].end.y);
        if (ret == 0)
            break;

        obstacles[l_ptr].start.name = l_ptr;
        obstacles[l_ptr].end.name   = l_ptr+1;
        obstacles[l_ptr].name       = l_ptr;
        l_ptr++;
    }
    fclose(input);
    //********************************************************************//
    //*******************END FILE READ**********************************//
    //********************************************************************//

    if (EXTRA_VERBOSE)
    {
        cout <<"Printing Points/obstacles/Boundaries" << endl;
        cout << "B0: (" << c1 << "," << c2 << "), (" << c3 << "," << c4 << ")"<< endl;

        cout <<"Printing Points :-\n";
        for (i=0;i<p_ptr;i++)
        {
            cout << i << '\t';
            points[i].PrintFull();
            cout << endl;
        }

        cout <<"Obstacles :-\n";
        for (i=0;i<l_ptr;i++)
        {
            cout<< i << '\t';
            obstacles[i].PrintLine();
        }
        cout <<endl;
    }


    //********************************************************************//
    //*******************CALCULATE DISTANCES******************************//
    //********************************************************************//
    d_ptr       =   0;
    edgeDistances   =   new Edge[p_ptr*p_ptr];

    if (OBSTACLE && EXTRA_VERBOSE)
        cout << "Points Obstructed :-" <<endl;

    for(i=0;i<p_ptr; i++)
    {
        for(j=i+1; j< p_ptr; j++)
        {
            if (OBSTACLE)
            {
                if (IsObstructed(points[i], points[j]))
                {
                    if (EXTRA_VERBOSE)
                    {
                        points[i].PrintFull();
                        cout << '\t';
                        points[j].PrintFull();
                        cout << endl;
                    }
                    continue;
                }
            }

            edgeDistances[d_ptr].start  = points[i];
            edgeDistances[d_ptr].end    = points[j];
            edgeDistances[d_ptr].CalculateDistance();
            edgeDistances[d_ptr].name = d_ptr;
            d_ptr++;

            //Note: For convenience during lookup operations, I add an edge twice into edge distances as below:-
            // Entry 1 Edge (P0, P1)
            // Entry 2 Edge (P1, P0)
            // This redundant entry makes the function PathExistLEQDistance() simpler

            edgeDistances[d_ptr].start  = points[j];
            edgeDistances[d_ptr].end    = points[i];
            edgeDistances[d_ptr].CalculateDistance();
            edgeDistances[d_ptr].name = d_ptr;
            d_ptr++;
        }
    }

    if (OBSTACLE)
        cout << "\nObstacle Detection On\n";
    else
        cout << "\nObstacle Detection Off\n";

    if (EXTRA_VERBOSE)
    {
        cout << "Raw Distances :-\n";
        for (i=0;i<d_ptr;i++)
        {
            cout << i << '\t';
            edgeDistances[i].PrintLine();
        }
    }

    //********************************************************************//
    //*******************SORT DISTANCES (Bubble sort)**********************//
    //********************************************************************//
    for(i=0;i< d_ptr; i++)
    {
        for(j=i+1; j< d_ptr; j++)
        {
            Edge a = edgeDistances[i];
            Edge b = edgeDistances[j];

            if (a.dist < b.dist)
            {
               edgeDistances[i] = b;
               edgeDistances[j] = a;
            }
        }
    }
    //Note: I considered using an O Log(n) sort but decided against the extra complexity,
    // since this is relatively performance inexpensive.

    if (EXTRA_VERBOSE)
    {
        cout << "\nSorted Distances :-\n";
        for (i=0;i<d_ptr;i++)
        {
            cout << i << '\t';
            edgeDistances[i].PrintLine();
        }
    }
}
//Compares if two Points are obstructed by any of the obstacles
bool IsObstructed(Point a, Point b)
{
    int i;
    for (i=0;i<l_ptr;i++)
    {
        Point c = Point(obstacles[i].start);
        Point d = Point(obstacles[i].end);
        if (IsObstructed(a,b,c,d))
            return true;
    }

    return false;
}

//Determines if theree points are clockwise
bool ClockWise(Point a, Point b, Point c)
{
    int side1 = (c.y - a.y) * (b.x-a.x) ;
    int side2 = (c.x-a.x)   * (b.y-a.y);

    if (side1 > side2)
        return false;
    else
        return true;
}

//Determines if the Line (a,b) is obstructed by the Line (c,d)
bool IsObstructed(Point a, Point b, Point c, Point d)
{
    bool acd = ClockWise(a,c,d);
    bool bcd = ClockWise(b,c,d);
    bool abc = ClockWise(a,b,c);
    bool abd = ClockWise(a,b,d);

    bool isOb;
    if (acd != bcd && abc != abd)
        isOb = true;
    else
        isOb = false;

    return  isOb;
}


/***************************************************************/
/****************Graphics Xlix Code******************************/
/***************************************************************/
void InitXLib(int argc, char **argv)
{
/* opening display: basic connection to X Server */
  if( (display_ptr = XOpenDisplay(display_name)) == NULL )
    { printf("Could not open display. \n"); exit(-1);}

  printf("Connected to X server  %s\n", XDisplayName(display_name) );

  screen_num 	 = DefaultScreen( display_ptr );
  screen_ptr 	 = DefaultScreenOfDisplay( display_ptr );
  color_map  	 = XDefaultColormap( display_ptr, screen_num );
  display_width  = DisplayWidth( display_ptr, screen_num );
  display_height = DisplayHeight( display_ptr, screen_num );

  printf("Width %d, Height %d, Screen Number %d\n",
           display_width, display_height, screen_num);

  /* creating the window */
  border_width = 10;
  win_x = 0; win_y = 0;
  win_width = display_width/2;
  win_height = (int) (win_width / 1.7); /*rectangular window*/


  win= XCreateSimpleWindow( display_ptr, RootWindow( display_ptr, screen_num),
                            win_x, win_y, win_width, win_height, border_width,
                            BlackPixel(display_ptr, screen_num),
                            WhitePixel(display_ptr, screen_num) );

  /* now try to put it on screen, this needs cooperation of window manager */
  size_hints 	= XAllocSizeHints();
  wm_hints 	= XAllocWMHints();
  class_hints 	= XAllocClassHint();

  if( size_hints == NULL || wm_hints == NULL || class_hints == NULL )
    { printf("Error allocating memory for hints. \n"); exit(-1);}

  size_hints -> flags = PPosition | PSize | PMinSize  ;
  size_hints -> min_width = 60;
  size_hints -> min_height = 60;

  XStringListToTextProperty( &win_name_string,1,&win_name);
  XStringListToTextProperty( &icon_name_string,1,&icon_name);

  wm_hints -> flags = StateHint | InputHint ;
  wm_hints -> initial_state = NormalState;
  wm_hints -> input = False;

  class_hints -> res_name = (char*)"x_use_example";
  class_hints -> res_class = (char*)"examples";

  XSetWMProperties( display_ptr, win, &win_name, &icon_name, argv, argc,
                    size_hints, wm_hints, class_hints );

  /* what events do we want to receive */
  XSelectInput( display_ptr, win,            ExposureMask | StructureNotifyMask | ButtonPressMask );

  /* finally: put window on screen */
  XMapWindow( display_ptr, win );

  XFlush(display_ptr);

  /* create graphics context, so that we may draw in this window */
  gc = XCreateGC( display_ptr, win, valuemask, &gc_values);
  XSetForeground( display_ptr, gc, BlackPixel( display_ptr, screen_num ) );
  XSetLineAttributes( display_ptr, gc, 1,LineOnOffDash, CapRound, JoinRound);

  //Yellow
  gc_yellow = XCreateGC( display_ptr, win, valuemask, &gc_yellow_values);
  XSetLineAttributes(display_ptr, gc_yellow, 1, LineSolid,CapRound, JoinRound);
  if( XAllocNamedColor( display_ptr, color_map, "yellow", &tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color yellow\n"); exit(-1);}
  else
    XSetForeground( display_ptr, gc_yellow, tmp_color1.pixel );

  //Red
  gc_red = XCreateGC( display_ptr, win, valuemask, &gc_red_values);
  XSetLineAttributes( display_ptr, gc_red, 2, LineSolid, CapRound, JoinRound);
  if( XAllocNamedColor( display_ptr, color_map, "red",&tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color red\n"); exit(-1);}
  else
    XSetForeground( display_ptr, gc_red, tmp_color1.pixel );

  //Grey
  gc_grey = XCreateGC( display_ptr, win, valuemask, &gc_grey_values);
  if( XAllocNamedColor( display_ptr, color_map, "light grey", &tmp_color1, &tmp_color2 ) == 0 )
    {printf("failed to get color grey\n"); exit(-1);}
  else
    XSetForeground( display_ptr, gc_grey, tmp_color1.pixel );

}

//Draws a line between point(a,b) and point(c,d)
void DrawLine(GC pGC, int a, int b, int c, int d)
{
  XDrawLine(display_ptr, win, pGC, a,b,c,d);
}

//Draws a circle centered at P(a,b)
//Note: The bounding rectangle of an arc refers to the smallest rectangle that completely contains the arc)
void DrawPoint(GC pGC, int a, int b)
{
    //Two unsigned integers to specify the width and height of the bounding rectangle of the arc
    unsigned int widthBoundingRect = 10;
    unsigned int heightBoundingRect = 10;

    //Two integers that specify the upper left corner of the bounding rectangle of the arc
    //Note: as the function is required to center the circle at a,b, we need to calculate the values of the upperlefCorner
    // as below
    int upperLeftCornerBoundingRecta = a-widthBoundingRect/2;
    int upperLeftCornerBoundingRectb = b-widthBoundingRect/2;

    //finally two integers to specify the start and extent of the arc.
    int startArc = 0;
    int extentArc = 360*64;

    XFillArc( display_ptr, win, pGC, upperLeftCornerBoundingRecta,upperLeftCornerBoundingRectb,widthBoundingRect,heightBoundingRect, startArc, extentArc);
}

//Draws all Obstacles, Points, and Edges
void Draw(int argc, char **argv)
{
    int i;    
    while(1)
    { XNextEvent( display_ptr, &report );
      switch( report.type )
        {
        case Expose:
          {
          for(i=0;i<p_ptr; i++)
              DrawPoint(gc_grey, points[i].x, points[i].y);


            for(i=0;i<lastPath.count; i++)
            {
                if (i+1 == lastPath.count)
                    break;
                DrawLine(gc,    lastPath.points[i].x,   lastPath.points[i].y,       lastPath.points[i+1].x,     lastPath.points[i+1].y);
            }

          for(i=0;i<l_ptr; i++)
              DrawLine(gc_red,      obstacles[i].start.x,   obstacles[i].start.y,       obstacles[i].end.x,         obstacles[i].end.y);
        }
          break;
        case ConfigureNotify:
          /* This event happens when the user changes the size of the window*/
          win_width     = report.xconfigure.width;
          win_height    = report.xconfigure.height;
          break;
        default:
          /* this is a catch-all for other events; it does not do anything.
             One could look at the report type to see what the event was */
          break;
        }

    }

}

