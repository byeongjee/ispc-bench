
#include <stdint.h>
#include <stdlib.h>

template <typename T, size_t N> struct MemRefDescriptor {
  T *allocated;
  T *aligned;
  intptr_t offset;
  intptr_t sizes[N];
  intptr_t strides[N];
};

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

#define WIDTH 900
#define HEIGHT 900
#define BASE_WIDTH 900
#define BASE_HEIGHT 900

typedef unsigned int uint;
using Mat4 = float[4][4];

void rt_serial(const Mat4 &raster2camera, const Mat4 &camera2world,
               float *__restrict__ image, int *__restrict__ id,
               const LinearBVHNode *__restrict__ nodes,
               const Triangle *__restrict__ triangles);

void _mlir_ciface_rt_serial(
    // const float raster2camera[__restrict__ 4][4],
    // const float camera2world[__restrict__ 4][4],
    MemRefDescriptor<float, 2> *raster2camera_ptr,
    MemRefDescriptor<float, 2> *camera2world_ptr,
    MemRefDescriptor<float, 2> *image_ptr, MemRefDescriptor<int, 2> *id,
    MemRefDescriptor<LinearBVHNode, 1> *nodes,
    MemRefDescriptor<Triangle, 1> *triangles);
}