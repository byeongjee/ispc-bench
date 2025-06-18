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

#define NX 256
#define NY 256
#define NZ 256
#define WIDTH 4

void stencil_serial(const float coef[5], const float vsq[], float Aeven[], float Aodd[]);

void _mlir_ciface_stencil_serial(MemRefDescriptor<float, 1> *coef, MemRefDescriptor<float, 1> *vsq, 
  MemRefDescriptor<float, 1> *Aeven, 
  MemRefDescriptor<float, 1> *Aodd);
}