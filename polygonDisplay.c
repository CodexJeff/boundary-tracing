#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>

#include "polygonSet.h"

#define SCALE 25

XPoint  pVertices[500];  //This is a fixed array that will hold the polygon vertices to be displayed.
XArc    pPoints[500];  //This is a fixed array that will hold the polygon vertices to be displayed.

// These are display-related variables
Display *display;
Window   win;
GC       gc;

// Draw the polygon set on the window.  This code assumes a maximum of 500 points per polygon
void drawGrid(char grid[30][30]) {
  // First erase background
  XSetForeground(display, gc, 0xFFFFFF);
  XFillRectangle(display, win, gc, 0, 0, 750, 750);
  XFlush(display);

  // Draw the grid
  for (int y=0; y<30; y++) {
    for (int x=0; x<30; x++) {
      if (grid[y][x] == '2')
      XSetForeground(display, gc, 0xFFFFFF);
      else if (grid[y][x] == '1')
      XSetForeground(display, gc, 0xCCCCCC);
      else
      XSetForeground(display, gc, 0x444444);
      XFillRectangle(display, win, gc, x*SCALE, y*SCALE, SCALE, SCALE);
    }
  }
  XFlush(display);
  getchar();
}

// Draw the polygon set on the window.  This code assumes a maximum of 500 points per polygon
void drawPolygonSet(char grid[30][30], PolygonSet *pSet) {
  Polygon *poly = pSet->firstPolygon;
  int      numVertices;

  // Draw the grid
  for (int y=0; y<30; y++) {
    for (int x=0; x<30; x++) {
      if (grid[y][x] == '2')
      XSetForeground(display, gc, 0xFFFFFF);
      else if (grid[y][x] == '1')
      XSetForeground(display, gc, 0xCCCCCC);
      else
      XSetForeground(display, gc, 0x444444);
      XFillRectangle(display, win, gc, x*SCALE, y*SCALE, SCALE, SCALE);
    }
  }
  XFlush(display);

  // Draw the polygon set
  while (poly != NULL) {
    // Go through the polygon's vertices and construct an XPoint
    numVertices = 0;
    Vertex *v = poly->firstVertex;
    while (v != NULL) {
      pVertices[numVertices].x = v->x*SCALE + SCALE/2;
      pVertices[numVertices].y = v->y*SCALE + SCALE/2;
      pPoints[numVertices].x = v->x*SCALE-SCALE/4 + SCALE/2;
      pPoints[numVertices].y = v->y*SCALE-SCALE/4 + SCALE/2;
      pPoints[numVertices].width = 10;
      pPoints[numVertices].height = 10;
      pPoints[numVertices].angle1 = 0;
      pPoints[numVertices].angle2 = 360*64;
      v = v->next;
      numVertices++;
    }
    if (numVertices > 2) {
      XSetForeground(display, gc, 0x000000);
      XDrawLines(display, win, gc, pVertices, numVertices, CoordModeOrigin);
      XFlush(display);
      XSetForeground(display, gc, 0xFF0000);
      XFillArcs(display, win, gc, pPoints, numVertices);
      XFlush(display);
    }
    poly = poly->next;
  }
  getchar();
}

int openDisplayWindow() {
  // Opens connection to X server
  display = XOpenDisplay(NULL);

  // Create a simple window
  win = XCreateSimpleWindow(display,                 // our connection to server
    RootWindow(display, 0),  // parent window (none in this example)
    0, 0,	             // x,y (w.r.t. parent ... ignored here)
    750,750,	             // width, height
    0, 		             // border width
    0x000000,                // border color (ignored in this example)
    0xFFFFFF);               // background color = WHITE

    // Set the name of the window
    XStoreName(display, win, "Polygon Displayer");
    
    // Get the graphics context
    gc = XCreateGC(display, win, 0, NULL);

    // Make it visible
    XMapWindow(display, win);
    XFlush(display);
    usleep(20000);  // sleep for 20 milliseconds.
  }

  int closeDisplayWindow() {
    // Clean up and close the window
    XFreeGC(display, gc);
    XUnmapWindow(display, win);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
  }
