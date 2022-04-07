#include "channel_descriptor.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "device_launch_parameters.h"
#include "foo.cuh"
#include <thrust/device_vector.h>
#include <utility>
//#include  "kernel.cuh"
//#include <unistd.h>
#include <stdio.h>
//#include <windows.h>   // WinApi header
#include <stdio.h>
// for __syncthreads()
#ifndef __CUDACC__
#    define __CUDACC__
#endif

#include <device_functions.h>

#ifdef __unix__
#    include <unistd.h>
#endif // __unix__
#include "progresscpp/ProgressBar.hpp"
extern int Min;
extern int Max;
extern float step;
extern int Size;
#define CHECK(res)                                                                       \
    {                                                                                    \
        if (res != cudaSuccess)                                                          \
        {                                                                                \
            printf("Error ：%s:%d , ", __FILE__, __LINE__);                              \
            printf("code : %d , reason : %s \n", res, cudaGetErrorString(res));          \
            exit(-1);                                                                    \
        }                                                                                \
    }

__device__ thrust::pair<float, float> find_ab(float *xpts, float *ypts, int i)
{
    float x1 = xpts[i * 2];
    float y1 = ypts[i * 2];
    float x2 = xpts[(i * 2) + 1];
    float y2 = ypts[(i * 2) + 1];

    //   printf("x1:%f y1:%f x2:%f y2:%f\n", x1, y1, x2, y2);
    float m = (y2 - y1) / (x2 - x1);
    float b = -m * x1 + y1;

    return thrust::make_pair(m, b);
}

__device__ bool isInside(float *xpts, float *ypts, int i, float x0, bool print = false)
{
    float x1 = xpts[i * 2];
    float x2 = xpts[(i * 2) + 1];
    if (print)
    {
        printf("s=%f xintersect=%f e=%f\n", x1, x0, x2);
    }

    if ((x1 > x2) && (x2 < x0) && (x0 < x1))
    {
        return true;
    }
    else if ((x1 < x2) && (x1 < x0) && (x0 < x2))
    {
        return true;
    }
    return false;
}

__global__ void findIntersection(float *xpts, float *ypts, int n, int batch, int start,count_t *d_mat,int Min,
                                 int Max,
                                 float step_,
                                 int Size)
{
    int i = threadIdx.x+start; // 0...batch size
    auto mb1 = find_ab(xpts, ypts, i);

    int step = (n/2-(i+1))/gridDim.x;
    int j = blockIdx.x*step+(i+1);
    int end = thrust::min(j+step,n/2);


    for (; j < end; j++)
    {
        auto mb2 = find_ab(xpts, ypts, j);
        double x_intersect = (mb2.second - mb1.second) / (mb1.first - mb2.first);
        if (isInside(xpts, ypts, j, x_intersect))
        {
            double yin = mb2.first * x_intersect + mb2.second;

            double x = 1/step_*(x_intersect+Max);
            double y = 1/step_*(yin+Max);
            int max_index = 2*Max*(int) (1/step_);
            if (y < 0 || y > max_index || x < 0 || x > max_index)
            {
                continue;
            }
            int xloc = __float2int_rn(x)*(float) max_index;
            int yloc = __float2int_rn(y);
            //            if(i==90 && j<200 && j>91){
            //                printf("%d %d %f %f x:%d y:%d\n",j+1,threadIdx.x + start+1,x_intersect,yin,__float2int_rn(x),yloc);
            //            }
            if(xloc+yloc>max_index*max_index)
                continue;
            atomicAdd(&d_mat[xloc+yloc],1.0f);

        }
    }
}

extern "C" float findValues(float *in_xpoints, float *in_ypoints, int num_pts,
    int batch_size, int start, float *xout, float *yout, count_t*mat)
{
    printf("%d\n",Min);
    float *d_pointsx;
    float *d_pointsy;

    float *d_xoutput;
    float *d_youtput;
    count_t * d_mat;

    int arraySize = sizeof(float) * num_pts;
    long int arraySize2 = sizeof(float) * num_pts * batch_size;

    //   printf("arraysize:%d\num_pts", arraySize2);

    CHECK(cudaMalloc((void **)&d_pointsx, arraySize));
    CHECK(cudaMalloc((void **)&d_pointsy, arraySize));
    CHECK(cudaMalloc((void **)&d_mat, sizeof(count_t) * Size * Size));
    CHECK(cudaMemcpy(d_mat, mat, sizeof(count_t) * Size * Size, cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(d_pointsx, in_xpoints, arraySize, cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(d_pointsy, in_ypoints, arraySize, cudaMemcpyHostToDevice));

    float time;
    cudaEvent_t startt, stop;

    cudaEventCreate(&startt);
    cudaEventCreate(&stop);
    cudaEventRecord(startt, 0);
    progresscpp::ProgressBar prog((num_pts / 2)/batch_size, 70);

    for (int s = 0; s < (num_pts / 2)-batch_size; s = s + batch_size)
    {
        findIntersection<<<10, batch_size>>>(d_pointsx, d_pointsy, num_pts,
            batch_size, s,d_mat,Min,Max,step,Size);

        prog.operator++();

        prog.display();
        CHECK(cudaDeviceSynchronize());
    }
    prog.done();
    //    findIntersection<<<grid, block>>>(d_points, num_pts / 2, d_xoutput, d_youtput);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, startt, stop);
    CHECK(cudaMemcpy(mat, d_mat, sizeof(count_t) * Size * Size, cudaMemcpyDeviceToHost));

    CHECK(cudaFree(d_mat));
    CHECK(cudaFree(d_pointsx));
    CHECK(cudaFree(d_pointsy));
    return time;
}
