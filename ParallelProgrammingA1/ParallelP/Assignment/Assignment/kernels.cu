
/// Kernal finds smallest value in input vector A, by using reduction pattern to compare two values and saving smallest.
/// This repeats until the total smallest value repeating until smallest value in data is stored in the first index of B
__global__ void reduce_find_min(const int* A, int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	int Z = blockDim.x;
	scratch[Y] = A[X];
	__syncthreads();
	for (int i = 1; i < Z; i *= 2) 
	{
		if (!(Y % (i * 2)) && ((Y + i) < Z)) 
			scratch[Y] = (scratch[Y] < scratch[Y + i]) ? scratch[Y] : scratch[Y + i];
		__syncthreads();
	}
	if (!Y) 
	{
		atomicMin(&B[0],scratch[Y]);
	}
}

/// Same kernal as the first one but reversed
__global__ void reduce_find_max(__device__ const int* A, __device__ int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	int Z = blockDim.x;
	scratch[Y] = A[X];
	__syncthreads();
	for (int i = 1; i < Z; i *= 2) 
	{
		if (!(Y % (i * 2)) && ((Y + i) < Z)) 
			scratch[Y] = (scratch[Y] > scratch[Y + i]) ? scratch[Y] : scratch[Y + i];
		__syncthreads();
	}
	if (!Y) 
	{
		atomicMax(&B[0],scratch[Y]);
	}
}

__global__ void reduce_find_sum(__device__ const int* A, __device__ int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	int Z = blockDim.x;
	scratch[Y] = A[X];
	__syncthreads();
	for (int i = 1; i < Z; i *= 2) 
	{
		if (!(Y % (i * 2)) && ((Y + i) < Z)) 
			scratch[Y] += scratch[Y + i];
		__syncthreads();
	}
	if (!Y) 
	{
		atomic_add(&B[0],scratch[Y]);
	}
}

__global__ void reduce_find_sum_variance(__device__ const int* A, __device__ int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	int Z = blockDim.x;
	scratch[Y] = A[X];
	__syncthreads();
	for (int i = 1; i < Z; i *= 2) 
	{
		if (!(Y % (i * 2)) && ((Y + i) < Z)) 
			scratch[Y] += scratch[Y + i];
		__syncthreads();
	}
	scratch[Y] = scratch[Y] / 10000.0f;
	if (!Y) 
	{
		atomic_add(&B[0],scratch[Y]);
	}
}

__global__ void find_variance(__device__ const int* A, __device__ int* B, int mean, int initialSize) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	if(X < initialSize)
	{ 
		B[X] = A[X] - mean;
		__syncthreads();
		B[X] = (B[X] * B[X]);
	}
}

__global__ void at_find_min(__device__ const int* A, __device__ int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	scratch[Y] = A[X];
	__syncthreads();
	atomicMin(&B[0],scratch[Y]);
}

__global__ void at_find_max(__device__ const int* A, __device__ int* B, __shared__ int* scratch) 
{
	int X = blockIdx.x * blockDim.x + threadIdx.x;
	int Y = threadIdx;
	scratch[Y] = A[X];
	__syncthreads(); 
	atomicMax(&B[0],scratch[Y]);
}