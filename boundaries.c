#include <stdio.h>
#include <stdlib.h>

#include "polygonSet.h"
#include "polygonDisplay.h"

#define WIDTH  30
#define HEIGHT 30

// Trace out a single boundary from the grid, starting at the given (xs,ys) location.  Mark the boundary
// locations with a 2 in the grid.  Assume that the polygon p has been created and has a single vertex at
// location (xs, ys).  The procedure will complete the polygon by adding the remaining boundary points it
// finds in sequence.
void traceWholeBorder(char grid[HEIGHT][WIDTH], int xs, int ys, Polygon *p) {
  int  xOffset[8] = {1, 1, 0, -1, -1, -1, 0, 1};
  int  yOffset[8] = {0, 1, 1, 1, 0, -1, -1, -1};

  int  xc = xs, yc = ys;  // The current point is the start point
  int  d = 7;

  char notDone = 1;
  while(notDone) {
    int dp;
    if (d%2 == 0)
    dp = (d+7)%8;
    else
    dp = (d+6)%8;
    int i;
    for (i=0; i<8; i++) {
      char val = grid[yc+yOffset[dp]][xc+xOffset[dp]];
      if (val != '0') {
        xc = xc + xOffset[dp];
        yc = yc + yOffset[dp];
        grid[yc][xc] = '2';

        Vertex *vert;
        vert = calloc(1, sizeof(Vertex)); // Creating dynamically allocated vertex
        vert -> x = xc; // Setting created vertex x
        vert -> y = yc; // Setting created vertex y

        // Setting created vertex to polygons vertexs
        p -> lastVertex -> next = vert;
        p -> lastVertex = vert;

        d = dp;
        break;
      }
      dp = (dp + 1)%8;
    }
    if (i == 8) {
      notDone = 0;
      grid[yc][xc] = '0'; // Ignore single points
    }
    if ((xc == xs) && (yc == ys))
    notDone = 0;
  }
}

// Compute the boundaries of the obstacles in the grid.  Add a polygon to pSet for each completed border.
void computeBoundaries(char grid[HEIGHT][WIDTH], PolygonSet *pSet) {
  Polygon *p;  // This will point to each polygon that you will trace

  for(int y=1; y<HEIGHT; y++) {
    for (int x=1; x<WIDTH; x++) {
      if ((grid[y][x] == '1') && (grid[y][x-1] == '0')) {

        p = calloc(1, sizeof(Polygon)); // Creating dynamically allocated polygon

        Vertex *vert;
        vert = calloc(1, sizeof(Vertex)); // Creating dynamically allocated vertex

        vert -> x = x; // Setting dynamically allocated x with grid x
        vert -> y = y; // Setting dynamically allocated y with grid y

        p -> firstVertex = vert; // Setting created vertex to dynamicalled allocated polygon firstVertex
        p -> lastVertex = vert; // Setting created vertex to dynamicalled allocated polygon lastVertex

        // Setting polygons and vertex to NULL
        vert -> next = NULL;
        p -> next = NULL;

        grid[y][x] = '2'; // Mark it as a border
        traceWholeBorder(grid, x, y, p);

        // If polygon is not first in set
        if (pSet -> lastPolygon != NULL){
          pSet -> lastPolygon -> next = p;
          pSet -> lastPolygon = p;
        }
        // If polygon first in set
        else{
          pSet -> firstPolygon = p;
          pSet -> lastPolygon = p;
        }
      }
    }
  }
}

// This procedure must clean up the polygons in the initialPolygons set by removing sequences of vertices
// that have the same x values or the same y values.
void cleanUpPolygons(PolygonSet *inputSet, PolygonSet *outputSet) {
  outputSet = inputSet;
  // Tried setting at least outputSet to vertice filled inputSet,
  // but not even that is appearing
}

// This procedure must free up all of the memory that was allocated to create the vertices, polygons and polygon set
void freePolygonSet(PolygonSet *pSet) {
  Polygon *memPolygon;
  Vertex *vertex;
  Vertex *memVertex;

  while (pSet -> firstPolygon != NULL){ // Loop for freeing the polygons
    memPolygon = pSet -> firstPolygon; // Keeping in memory
    vertex =  pSet -> firstPolygon -> firstVertex; // Setting current vertex
    pSet -> firstPolygon = pSet -> firstPolygon -> next; // Setting polygon to next
    free(memPolygon); // Freeing up the stored
    while (vertex != NULL){ // Loop for freeing the verticies
      memVertex = vertex; // Keeping in memory
      vertex = vertex -> next; // Setting current vertex to next
      free(memVertex); // Freeing up "previous"
    }
  }
}

// Reset the set by freeing its polygons and starting as an empty set again
void resetPolygonSet(PolygonSet *pSet) {
  freePolygonSet(pSet);
  pSet->firstPolygon = NULL;
  pSet->lastPolygon = NULL;
}

// This is where it all begins
int main() {
  // Allocate a PolygonSet to store the original traced border points
  PolygonSet *initPolygons = malloc(sizeof(PolygonSet));
  initPolygons->firstPolygon = NULL;
  initPolygons->lastPolygon = NULL;

  // Allocate a PolygonSet to store the simplified/processed polygons
  PolygonSet *finalPolygons = malloc(sizeof(PolygonSet));
  finalPolygons->firstPolygon = NULL;
  finalPolygons->lastPolygon = NULL;

  char grids[5][HEIGHT][WIDTH] = {
    {"000000000000000000000000000000",
    "000111111111111111111111110000",
    "000111111111111111111111110000",
    "000111111110000000111111110000",
    "000111111110000000111111110000",
    "000111111110000000111111110000",
    "000111111110000000111111110000",
    "000111111110000000111111110000",
    "000111111110000000111111110000",
    "000111111111111111111111110000",
    "000111111111111111111111110000",
    "000011111111111111111111100000",
    "000001111111111111111111000000",
    "000000111111111111111110000000",
    "000000011111111111111100000000",
    "000000001111111111111000000000",
    "000000000000111110000000000000",
    "000000000000111110000000000000",
    "000000000000111110000000000000",
    "001111111111111111111111111100",
    "001111111111111111111111111100",
    "001111111111000000111111111100",
    "001111111111000000111111111100",
    "001111111111000000111111111100",
    "001111111111000000111111111100",
    "000011111000000000000111000000",
    "000011111000000000000111000000",
    "000011111000000000000111000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000"},

    {"000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000000111111000000000000000",
    "000000011111111111110000000000",
    "000001111111111000000000110000",
    "000001111111000000000001110000",
    "000001111111000000000000111000",
    "000000011111000000000000000000",
    "000000011111111000000000000000",
    "000000000111111110000000000000",
    "000000111111111111111100000000",
    "000111111111111111111111000000",
    "001111111111111111111000000000",
    "000111111100011111111110000000",
    "011111100000111111111000000000",
    "001111110000000011111111000000",
    "001111110000000011111100000000",
    "000011110000000111111000000000",
    "000001110000000111000000000000",
    "000000010000000110000000000000",
    "000000000000000000000000110000",
    "000000000000000000000000000000",
    "000000011111111110000000000000",
    "001000000011111111110000001000",
    "001000000000011111111000000000",
    "001000000000001111111100000000",
    "001000000001111110000000001000",
    "001111000000011110000000011000",
    "000000000000000110000000000000",
    "000000000000000000000000000000"},

    {"000000000000000000000000000000",
    "001110001110000110001000100100",
    "001110001110000110001000100000",
    "001110001110000110001000000000",
    "000000000000000000000000000000",
    "001111111111111111111111111100",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000000011111111111111100000000",
    "000000010101010101010100000000",
    "000000010101010101010100000000",
    "000000010101010101010100000000",
    "000000000000000000000000000000",
    "000000100000000000000010000000",
    "000000010000011100000100000000",
    "000000001000001000001000000000",
    "000000000100001000010000000000",
    "000000000010000000100000000000",
    "000000000001000001000000000000",
    "000000000000100010000000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000",
    "000001111111111111111111100000",
    "000001111110000000000000000000",
    "000000111111111100000000000000",
    "000000001111111111110000000000",
    "000000000001111111111110000000",
    "000000000000000111111111100000",
    "000000000000000000001111110000",
    "000000000000000000000000000000"},

    {"000000000000000000000000000000",
    "001111111111111111111111111100",
    "001111111111111111111111111100",
    "001111111111111111111111111100",
    "001110000000000000000000000000",
    "001110111111111111111111111110",
    "001110111111111111111111111110",
    "001110111111111111111111111110",
    "001110111000000000000000001110",
    "001110111011111111111111101110",
    "001110111011111111111111101110",
    "001110111011111111111111101110",
    "001110111011100000000011101110",
    "001110111011101111111011101110",
    "001110111011101111111011101110",
    "001110111011101111111011101110",
    "001110111011101110111011101110",
    "001110111011100000111011101110",
    "001110111011111111111011101110",
    "001110111011111111111011101110",
    "001110111011111111111011101110",
    "001110111000000000000011101110",
    "001110111111111111111111101110",
    "001110111111111111111111101110",
    "001110111111111111111111101110",
    "001110000000000000000000001110",
    "001111111111111111111111111110",
    "001111111111111111111111111110",
    "001111111111111111111111111110",
    "000000000000000000000000000000"},

    {"000000000000000000000000000000",
    "000001100000000000000000111000",
    "000011110000000000000011110000",
    "000111111000000000011111100000",
    "000111111000000111111110000000",
    "000001111000001111111000000000",
    "000000011000111111000000000000",
    "000000001000001111000000000000",
    "000000000000000011000000000000",
    "000111100000000000000000000000",
    "000111000000000000000000000000",
    "000111000001111111111111110000",
    "000111000000111111111111110000",
    "000011000001111111111111110000",
    "000011000000001111111111110000",
    "000011000001111111111111110000",
    "000010000001111100000000000000",
    "000010000001111100000000000000",
    "000010000000111110000000000000",
    "000011000000011111111111100000",
    "000011100000000011111111100000",
    "000001100000000000111111100000",
    "000001111000000000000111100000",
    "000000111000000000000111000000",
    "000000011000000000000111000000",
    "000000000000011111111111000000",
    "000000000000011111111100000000",
    "000000000000000000111000000000",
    "000000000000000000000000000000",
    "000000000000000000000000000000"}};

    // Open a display window
    openDisplayWindow();

    // Run the tests on the 5 grids
    for (int i=0; i<5; i++) {
      // Draw the original grid
      drawGrid(grids[i]);

      // Compute the boundaries and redraw the grid
      computeBoundaries(grids[i], initPolygons);
      drawGrid(grids[i]);

      // Now draw the boundaries as polygons
      drawPolygonSet(grids[i], initPolygons);

      // Clean up the polygons and redraw them
      cleanUpPolygons(initPolygons, finalPolygons);
      drawPolygonSet(grids[i], finalPolygons);

      // Reset to get reday for the next grid test
      resetPolygonSet(initPolygons);
      resetPolygonSet(finalPolygons);
    }

    // Free up the allocated memory
    free(initPolygons);
    free(finalPolygons);

    // Close the display window
    closeDisplayWindow();
  }
