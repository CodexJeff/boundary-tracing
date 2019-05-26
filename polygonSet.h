// This struct represents a single vertex of a polygon
typedef struct vert {
  int     x;
  int     y;
  struct vert *next;
} Vertex;

// This struct represents a polygon composed of a chain of several vertices
typedef struct poly {
  Vertex  *firstVertex;
  Vertex  *lastVertex;
  struct poly *next;
} Polygon;

// This struct represents a sequence of polygons
typedef struct {
  Polygon  *firstPolygon;
  Polygon  *lastPolygon;
} PolygonSet;
