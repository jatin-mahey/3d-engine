#include <stdint.h>
#include <math.h>


typedef struct vector3
{
    float x;
    float y;
    float z;
} vec3;


typedef struct triangle
{
    vec3 t[3];
} tri;


typedef struct matrix4x4
{
    float m[4][4];
} mat4x4;


void magnitude       ( vec3* vi, float* so );
void invert          ( vec3* vi, vec3* vo );
void normalize       ( vec3* vi, vec3* vo );
void addScaler       ( vec3* vi, float si, vec3* vo );
void subScaler       ( vec3* vi, float si, vec3* vo );
void divScaler       ( vec3* vi, float si, vec3* vo );
void mulScaler       ( vec3* vi, float si, vec3* vo );
void dotProduct      ( vec3* v1, vec3* v2, vec3* vo );
void crossProduct    ( vec3* v1, vec3* v2, vec3* vo );
void mulmatrix       ( vec3* vi, mat4x4* m, vec3* vo );
void projection      ( mat4x4* m, float screen_w, float screen_h );
