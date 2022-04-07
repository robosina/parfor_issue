#ifndef FOO_CUH
#define FOO_CUH

#include <stdio.h>
#include "channel_descriptor.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "device_launch_parameters.h"
//#include <unistd.h>
#include <stdio.h>
typedef unsigned long long count_t;

extern "C" float findValues(float *in_xpoints, float *in_ypoints, int num_pts,
    int batch_size, int start, float *xout, float *yout, count_t *mat);

#endif
