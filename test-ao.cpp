/*
  Copyright (c) 2010-2011, Intel Corporation
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <malloc.h>
#endif
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <sys/types.h>

#include "timing.h"
#include "ao_serial.h"

#define NSUBSAMPLES 2

static unsigned int width, height;
static unsigned char *img;
static float *fimg;

static unsigned char clamp(float f) {
    int i = (int)(f * 255.5);

    if (i < 0)
        i = 0;
    if (i > 255)
        i = 255;

    return (unsigned char)i;
}

static void savePPM(const char *fname, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            img[3 * (y * w + x) + 0] = clamp(fimg[3 * (y * w + x) + 0]);
            img[3 * (y * w + x) + 1] = clamp(fimg[3 * (y * w + x) + 1]);
            img[3 * (y * w + x) + 2] = clamp(fimg[3 * (y * w + x) + 2]);
        }
    }

    FILE *fp = fopen(fname, "wb");
    if (!fp) {
        perror(fname);
        exit(1);
    }

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "255\n");
    fwrite(img, w * h * 3, 1, fp);
    fclose(fp);
    printf("Wrote image file %s\n", fname);
}



int main() {
  // Allocate space for output images
  img = new unsigned char[WIDTH * HEIGHT * 3];
  fimg = new float[WIDTH * HEIGHT * 3];

  MemRefDescriptor<float, 3> fimg_desc = {
      fimg, fimg, 0,
      {HEIGHT, WIDTH, 3}, // sizes
      {WIDTH * 3, 3, 1}   // strides
  };

  memset((void *)fimg, 0, sizeof(float) * WIDTH * HEIGHT * 3);
  reset_and_start_timer();
  #ifdef INTRINSIC_COMPILER
    _mlir_ciface_ao_serial(NSUBSAMPLES, &fimg_desc);
  #else
    ao_serial(NSUBSAMPLES, fimg);
  #endif
  double t = get_elapsed_mcycles();
  printf ("[execution time] %0.6f\n", t);

  savePPM("ao-serial.ppm", WIDTH, HEIGHT);

  return 0;
}
