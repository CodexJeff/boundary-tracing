#include <X11/Xlib.h>
// Draw the polygon set on the window.
extern void drawGrid(char grid[30][30]);

// Draw the polygon set on the window.
extern void drawPolygonSet(char grid[30][30], PolygonSet *pSet);

// Open a display window
extern int openDisplayWindow();

// Close a display window
extern int closeDisplayWindow();
