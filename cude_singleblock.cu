#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <numeric>
#include <time.h>
#define col 48
#define row 30

using namespace std;

__global__ void MatrixAvg(float* input)
{
	int tid = threadIdx.x;
	auto step_size = 1;
	int threads_num = blockDim.x;

	while (threads_num > 0)
	{
		if (tid < threads_num)
		{
			const auto fst = tid * step_size * 2;
			const auto snd = fst + step_size;
			input[fst] += input[snd];
		}

		step_size <<= 1;
		threads_num >>= 1;
	}
}

int main()
{
  srand(time(0));
	int N = col * row;
	float** arr, * dev_arr;
	arr = new float * [row];
	arr[0] = new float [N];
	for (int i = 1; i < col;i++)
	{
		arr[i] = arr[i - 1] + col;
	}

	cudaMalloc(&dev_arr, N * sizeof(float));

	for (int i = 0; i < N; i++) {
		arr[0][i] = rand();
	}

	cudaMemcpy(dev_arr, arr[0], N * sizeof(float), cudaMemcpyHostToDevice);

	int blocksize = 1024;
	int gridsize = 24;

	MatrixAvg <<<gridsize,blocksize>>>(dev_arr);
	int out;
	cudaMemcpy(&out, dev_arr, N * sizeof(float), cudaMemcpyDeviceToHost);

	cout << "Average is:" << out/N << endl;

	cudaFree(dev_arr);
	delete(arr[0]);
	delete(arr);

	return 0;
}
