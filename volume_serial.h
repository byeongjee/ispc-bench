#define WIDTH 896
#define HEIGHT 1184

extern "C" {

using Mat4 = float[4][4];
void volume_serial(float *__restrict__ density, int *__restrict__ nVoxels,
                   const Mat4 &raster2camera, const Mat4 &camera2world,
                   int width, int height, float *__restrict__ image);
}
