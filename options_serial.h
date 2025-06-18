#include <stdint.h>
#include <stdlib.h>
#ifndef OPTIONS_SERIAL_H
#define OPTIONS_SERIAL_H

template<typename T, size_t N>
struct MemRefDescriptor {
  T *allocated;
  T *aligned;
  intptr_t offset;
  intptr_t sizes[N];
  intptr_t strides[N];
};

extern "C" {
void options_serial(float Sa[], float Xa[], float Ta[], float ra[], float va[],
                    float result[], int count, bool isBlackScholes);

void _mlir_ciface_options_serial(MemRefDescriptor<float, 1> *Sa,
                                 MemRefDescriptor<float, 1> *Xa,
                                 MemRefDescriptor<float, 1> *Ta,
                                 MemRefDescriptor<float, 1> *ra,
                                 MemRefDescriptor<float, 1> *va,
                                 MemRefDescriptor<float, 1> *result, int count,
                                 bool isBlackScholes);
}
#endif