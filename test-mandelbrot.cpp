#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include "timing.h"
#include "mandelbrot_serial.h"

/* Write a PPM image file with the image of the Mandelbrot set */
static void writePPM(int *buf, int width, int height, const char *fn) {
    FILE *debug_fp = fopen("debug.txt", "w");
    FILE *fp = fopen(fn, "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int i = 0; i < width * height; ++i) {
        // Map the iteration count to colors by just alternating between
        // two greys.
        char c = (buf[i] & 0x1) ? (char)240 : 20;
        for (int j = 0; j < 3; ++j) {
          fprintf(debug_fp, "%d ", (int)c);
            fputc(c, fp);
        }
        fputc('\n', debug_fp);
    }
    fclose(fp);
    fclose(debug_fp);
    printf("Wrote image file %s\n", fn);
}

int main() {
    unsigned int width = 768;
    unsigned int height = 512;

    int *buf = new int[width * height];
    MemRefDescriptor<int, 2> buf_desc = {
        .allocated = buf,
        .aligned = buf,
        .offset = 0,
        .sizes = {width, height},
        .strides = {width, 1}
    };

    // Clear out the buffer
    for (unsigned int i = 0; i < width * height; ++i)
        buf[i] = 0;

    reset_and_start_timer();
    #ifdef INTRINSIC_COMPILER
        _mlir_ciface_mandelbrot_serial(&buf_desc);
    #else
        mandelbrot_serial(buf);
    #endif
    double dt = get_elapsed_mcycles();
  printf ("[execution time] %0.6f\n", dt);

    writePPM(buf, width, height, "mandelbrot-serial.ppm");

    return 0;
}
