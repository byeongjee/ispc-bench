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
void mandelbrot_serial(int output[]);
void _mlir_ciface_mandelbrot_serial(MemRefDescriptor<int, 2> *input);
}