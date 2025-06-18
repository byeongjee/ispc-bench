#include <stdint.h>
#include <stdlib.h>

template<typename T, size_t N>
struct MemRefDescriptor {
  T *allocated;
  T *aligned;
  intptr_t offset;
  intptr_t sizes[N];
  intptr_t strides[N];
};

extern "C" {

void volume_serial(float *__restrict__ density, int *__restrict__ nVoxels, const float *__restrict__ raster2camera_ptr, const float *__restrict__ camera2world_ptr,
                   int width, int height, float *__restrict__ image);
void _mlir_ciface_volume_serial(MemRefDescriptor<float, 1>* density, MemRefDescriptor<int, 1>* n,
    MemRefDescriptor<float, 2>* raster2camera_ptr, MemRefDescriptor<float, 2>* camera2world_ptr,
                   int width, int height, MemRefDescriptor<float, 1>* image);
}