#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define BIAS 1

#define DATA_X 4
#define DATA_Y 4

#define N_LAYERS 6
#define I_N_NEURONS DATA_Y
#define H_N_NEURONS 4
#define O_N_NEURONS 1

#define EULER_NUMBER 2.71828


typedef enum layer_type {
	INPUT,
	HIDDEN,
	OUTPUT,
} layer_type;

#endif