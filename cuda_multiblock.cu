#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <numeric>
#include<time.h>
#define col 20000
#define row 10000

using namespace std;

static const int N = col * row;
static const int blockSize = 1024;
static const int gridSize = 24;

__global__ void MatrixAvg(const int* gArr, int arraySize, int* gOut) {
    int thIdx = threadIdx.x;
    int gthIdx = thIdx + blockIdx.x * blockSize;
    const int gridSize = blockSize * gridDim.x;
    int sum = 0;
    for (int i = gthIdx; i < arraySize; i += gridSize)
        sum += gArr[i];
    __shared__ int shArr[blockSize];
    shArr[thIdx] = sum;
    __syncthreads();
    for (int size = blockSize / 2; size > 0; size /= 2) { //uniform
        if (thIdx < size)
            shArr[thIdx] += shArr[thIdx + size];
        __syncthreads();
    }
    if (thIdx == 0)
        gOut[blockIdx.x] = shArr[0];
}


int main()
{
    srand(time(0));
    int **arr;
    int *dev_arr;

    arr = new int *[row];
    arr[0] = new int [N];
    for (int i = 1; i < col;i++)
    {
        arr[i] = arr[i - 1] + col;
    }
    cudaMalloc((void**)&dev_arr, N * sizeof(int));
    cudaMemcpy(dev_arr, arr, N * sizeof(int), cudaMemcpyHostToDevice);
    for (int i = 0; i < N; i++)
    {
        arr[0][i] = rand();
    }

    int out;
    int *dev_out;
    cudaMalloc((void**)&dev_out, sizeof(int) * gridSize);

    MatrixAvg << <gridSize, blockSize >> > (dev_arr, N, dev_out);
    //dev_out now holds the partial result
    MatrixAvg << <1, blockSize >> > (dev_out, gridSize, dev_out);
    //dev_out[0] now holds the final result
    cudaDeviceSynchronize();

    cudaMemcpy(&out, dev_out, sizeof(int), cudaMemcpyDeviceToHost);
    cout << "Average is:" << out/N << endl;
    cudaFree(dev_arr);
    cudaFree(dev_out);
    return 0;
}
