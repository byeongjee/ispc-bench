/*
  Copyright (c) 2010-2014, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#endif

#include "timing.h"
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "stencil_serial.h"


void InitData(int Nx, int Ny, int Nz, float *A[2], float *vsq) {
    int offset = 0;
    for (int z = 0; z < Nz; ++z)
        for (int y = 0; y < Ny; ++y)
            for (int x = 0; x < Nx; ++x, ++offset) {
                A[0][offset] = (x < Nx / 2) ? x / float(Nx) : y / float(Ny);
                A[1][offset] = 0;
                vsq[offset] = x * y * z / float(Nx * Ny * Nz);
            }
}

int main() {
  float* Aserial_even = new float[NX * NY * NZ];
  float* Aserial_odd = new float[NX * NY * NZ];
  float *vsq = new float[NX * NY * NZ];


  float coeff[4] = {0.5, -.25, .125, -.0625};

  float* Aserial[2] = {Aserial_even, Aserial_odd};
  InitData(NX, NY, NZ, Aserial, vsq);

  reset_and_start_timer();
  stencil_serial(coeff, vsq, Aserial[0], Aserial[1]);
  double dt = get_elapsed_mcycles();
  printf ("[execution time] %0.6f\n", dt);

  const char *refOut;
  #if defined(CLANG12_COMPILER)
    *refOut = "stencil-clang12.bin";
  #elif defined(CLANG12_WITHOUT_VEC_COMPILER)
    *refOut = "stencil-clang12-without-vec.bin";
  #elif defined(CLANG18_COMPILER)
    *refOut = "stencil-clang18.bin";
  #elif defined(CLANG18_WITHOUT_VEC_COMPILER)
    *refOut = "stencil-clang18-without-vec.bin";
  #elif defined(VEGEN_COMPILER)
    *refOut = "stencil-vegen.bin";
  #elif defined(INTRINSIC_COMPILER)
    *refOut = "stencil-intrinsic.bin";
  #endif

  FILE *f = fopen(refOut, "wb");
  if (f) {
      size_t n = NX * NY * NZ;
      fwrite(Aserial_even, sizeof(float), n, f);
      fclose(f);
      printf("Reference field written: %s (%.1f MB)\n",
              refOut, n*sizeof(float)/1.0e6);
  } else {
      puts("Failed to open output file!");
  }

  return 0;
}
