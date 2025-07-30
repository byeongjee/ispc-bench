
#include <stdint.h>
#include <stdlib.h>

extern "C" {

struct Triangle {
  float p[3][4]; // extra float pad after each vertex
  int32_t id;
  int32_t pad[3]; // make 16 x 32-bits
};

struct LinearBVHNode {
  float bounds[2][3];
  int32_t offset; // primitives for leaf, second child for interior
  uint8_t nPrimitives;
  uint8_t splitAxis;
  uint16_t pad;
};

// sponza dataset
#define WIDTH 900
#define HEIGHT 900
#define BASE_WIDTH 900
#define BASE_HEIGHT 900

// cornell dataset
// #define WIDTH 248
// #define HEIGHT 248
// #define BASE_WIDTH 248
// #define BASE_HEIGHT 248

typedef unsigned int uint;
using Mat4 = float[4][4];

 void rt_serial(
    const Mat4 &raster2camera,
    const Mat4 &camera2world,
    float *__restrict__ image, int *__restrict__ id,
    // Triangle
    const float * p,
    const int32_t* triangleId,
    const int32_t* trainglePad,
    // LinearBVHNode
    const float* bounds,
    const int32_t* offset,
    const uint8_t* nPrimitives,
    const uint8_t* splitAxis,
    const uint16_t* nodePad
 );
}