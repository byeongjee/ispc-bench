#define WIDTH 896
#define HEIGHT 1184

extern "C" {

void volume_serial(float *__restrict__ density, int *__restrict__ nVoxels, const float *__restrict__ raster2camera_ptr, const float *__restrict__ camera2world_ptr,
                   int width, int height, float *__restrict__ image);
}