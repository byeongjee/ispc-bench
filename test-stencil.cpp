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

  MemRefDescriptor<float, 1> coeff_desc = {.allocated = coeff,
                                           .aligned = coeff,
                                           .offset = 0,
                                           .sizes = {4},
                                           .strides = {1}};
  MemRefDescriptor<float, 1> vsq_desc = {.allocated = vsq,
                                         .aligned = vsq,
                                         .offset = 0,
                                         .sizes = {NX * NY * NZ},
                                         .strides = {1}};
  MemRefDescriptor<float, 1> Aeven_desc = {.allocated = Aserial_even,
                                           .aligned = Aserial_even,
                                           .offset = 0,
                                           .sizes = {NX * NY * NZ},
                                           .strides = {1}};
  MemRefDescriptor<float, 1> Aodd_desc = {.allocated = Aserial_odd,
                                          .aligned = Aserial_odd,
                                          .offset = 0,
                                          .sizes = {NX * NY * NZ},
                                          .strides = {1}};

  float* Aserial[2] = {Aserial_even, Aserial_odd};
  InitData(NX, NY, NZ, Aserial, vsq);

  reset_and_start_timer();
  #ifdef INTRINSIC_COMPILER
    _mlir_ciface_stencil_serial(0, 6, WIDTH, NX - WIDTH, WIDTH, NY - WIDTH, WIDTH, NZ - WIDTH,
                                NX, NY, NZ, &coeff_desc, &vsq_desc, &Aeven_desc, &Aodd_desc);
  #else
    stencil_serial(0, 6, WIDTH, NX - WIDTH, WIDTH, NY - WIDTH, WIDTH, NZ - WIDTH,
                  NX, NY, NZ, coeff, vsq, Aserial[0], Aserial[1]);
  #endif
  double dt = get_elapsed_mcycles();
  printf("@time of serial run:\t\t\t[%.3f] million cycles\n", dt);

  return 0;
}
