#include <math.h>
#include "draw.h"


void invert ( vec3* vi,  vec3* vo )
{
    vo->x = -1*(vi->x);
    vo->y = -1*(vi->y);
    vo->z = -1*(vi->z);
}


void magnitude ( vec3* vi, float* so )
{
    vec3 vo;
    dotProduct(vi, vi, &vo);
    *so = sqrtf(vo.x + vo.y + vo.z);
}


void normalize ( vec3* vi, vec3* vo )
{
    float m;
    magnitude(vi, &m);
    if (m > 0) divScaler(vi, m, vo);
}


void addScaler ( vec3* vi, float si, vec3* vo )
{
    vi->x = vo->x + si;
    vi->y = vo->y + si;
    vi->z = vo->z + si;
}


void subScaler ( vec3* vi, float si, vec3* vo )
{
    vi->x = vo->x - si;
    vi->y = vo->y - si;
    vi->z = vo->z - si;
}


void mulScaler ( vec3* vi, float si, vec3* vo )
{
    vi->x = vo->x * si;
    vi->y = vo->y * si;
    vi->z = vo->z * si;
}


void divScaler ( vec3* vi, float si, vec3* vo )
{
    vi->x = vo->x / si;
    vi->y = vo->y / si;
    vi->z = vo->z / si;
}


void dotProduct ( vec3* v1, vec3* v2, vec3* vo )
{
    vo->x = v1->x * v2->x;
    vo->y = v1->y * v2->y;
    vo->z = v1->z * v2->z;
}


void crossProduct ( vec3* v1, vec3* v2, vec3* vo )
{
    vo->x = v1->y*v2->z - v1->z*v2->y;
    vo->y = v1->z*v2->x - v1->x*v2->z;
    vo->z = v1->x*v2->y - v1->y*v2->x;
}


void mulmatrix ( vec3* vi, mat4x4* m, vec3* vo )
{
    float w;
    vo->x = vi->x * m->m[0][0] + vi->y * m->m[1][0] + vi->z * m->m[2][0] + m->m[3][0];
    vo->y = vi->x * m->m[0][1] + vi->y * m->m[1][1] + vi->z * m->m[2][1] + m->m[3][1];
    vo->z = vi->x * m->m[0][2] + vi->y * m->m[1][2] + vi->z * m->m[2][2] + m->m[3][2];
    w     = vi->x * m->m[0][3] + vi->y * m->m[1][3] + vi->z * m->m[2][3] + m->m[3][3];

    if (w != 0.0)
    {
        divScaler(vo, w, vo);
    }
}


void projection ( mat4x4* m, float screen_w, float screen_h)
{
    float near = 0.1f;
    float far  = 1000.0f;
    float fov  = 90.0f;
    float fovRad = 1.0f / tanf(fov * 0.5 / 180.0 * 3.14159);
    float ratio = screen_h / screen_w;

    m->m[0][0] = ratio * fovRad;
    m->m[1][1] = fovRad;
    m->m[2][2] = far / (far - near);
    m->m[3][2] = (-far * near)/(far - near);
    m->m[2][3] = 1.0;
    m->m[3][3] = 0.0;
}

