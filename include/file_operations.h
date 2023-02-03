#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vect3D.h"
#include "engine.h"


int count_lines_columns(FILE* f,int* columns);

vect3D** read_file(FILE* f,int* rows,int* columns);

#endif