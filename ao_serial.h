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
  
void ao_serial(int nsubsamples, float image[]);
#define WIDTH 1024
#define HEIGHT 1024

void _mlir_ciface_ao_serial(int nsubsamples, MemRefDescriptor<float, 3> *input);
}
