#define WIDTH 896
#define HEIGHT 1184
#define NUM_DENSITY 147456
#define LEN_IMAGE WIDTH * HEIGHT * 3

extern "C" {
#ifdef VEGEN_COMPILER
void volume_serial(float *__restrict__ density, int *__restrict__ nVoxels,
                   const float *__restrict__ raster2camera_ptr,
                   const float *__restrict__ camera2world_ptr, int width,
                   int height, float *__restrict__ image);
#else
void volume_serial(float density[NUM_DENSITY], int nVoxels[3],
                   const float raster2camera[4][4],
                   const float camera2world[4][4], int width, int height,
                   float image[LEN_IMAGE]);
#endif
}
