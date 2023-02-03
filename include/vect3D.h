#ifndef VECT3D_H
#define VECT3D_H

#include <stdio.h>

struct vect3D {
    float x;
    float y;
    float z;
};

typedef struct vect3D vect3D;


void print_vect3D(vect3D v);

#endif