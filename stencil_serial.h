extern "C" {

#define NX 256
#define NY 256
#define NZ 256
#define WIDTH 4

void stencil_serial(const float coef[5], const float vsq[], float Aeven[], float Aodd[]);
}