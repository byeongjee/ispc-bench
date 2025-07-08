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
    // Clear out the buffer
    for (unsigned int i = 0; i < width * height; ++i)
        buf[i] = 0;

    reset_and_start_timer();
    mandelbrot_serial(buf);
    double dt = get_elapsed_mcycles();
    printf ("[execution time] %0.6f\n", dt);

    #if defined(CLANG12_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-clang12.ppm");
    #elif defined(CLANG12_WITHOUT_VEC_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-clang12-without-vec.ppm");
    #elif defined(CLANG18_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-clang18.ppm");
    #elif defined(CLANG18_WITHOUT_VEC_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-clang18-without-vec.ppm");
    #elif defined(VEGEN_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-vegen.ppm");
    #elif defined(INTRINSIC_COMPILER)
      writePPM(buf, width, height, "mandelbrot-serial-intrinsic.ppm");
    #endif

    return 0;
}
