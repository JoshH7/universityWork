//First kernel uses a reduction pattern to compair two values in input vector A and saves the biggest one
//This is repeated until the max of the dataset is found and saved in the first index of vector B
__kernel void reduceMethodMax(__global const int* A, __global int* B, __local int* C)
{
	//getting ID's to get the index of the current work items
	int sL = get_local_size(0);
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL] = A[id];
	barrier(CLK_LOCAL_MEM_FENCE);

	for (int i = 1; i < N; i *= 2) {
		if (!(idL % (i * 2)) && ((idL + i) < N))
		{
			if (C[idL] > C[idL + i])
				C[idL] = C[idL + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	if (!idL)
	{
		atomic_max(&B[0], C[idL]);
	}
}
//This kernal is the inverse of the first one, meaning it outputs the minimum value
__kernel void reduceMethodMin(__global const int* A, __global int* B, __local int* C)
{
	int sL = get_local_size(0);
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL] = A[id];
	barrier(CLK_LOCAL_MEM_FENCE);
	for (int i = 1; i < N; i *= 2) {
		if (!(idL % (i * 2)) && ((idL + i) < N))
		{
			if (C[idL] < C[idL + i])
				C[idL] = C[idL + i];
		}
		
			C[idL] = (C[idL] < C[idL + i]) ? C[idL] : C[idL + i];
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	if (!idL)
	{
		atomic_min(&B[0], C[idL]);
	}
}
//Similar kernal again but producing the sum
__kernel void reduceMethodSum(__global const int* A, __global int* B, __local int* C)
{
	int sL = get_local_size(0);
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL ] = A[id];
	barrier(CLK_LOCAL_MEM_FENCE);
	for (int i = 1; i < N; i *= 2)
	{
		if (!(idL % (i * 2)) && ((idL + i) < N))
			C[idL ] += C[idL + i];
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	if (!idL )
	{
		atomic_add(&B[0], C[idL ]);
	}
}

//Kernal to find the sum of B/C, performing an attomic add both allowes for higher performance and is uninteruptable
__kernel void reduceFindSumVar(__global const int* A, __global int* B, __local int* C)
{
	int sL = get_local_size(0);
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL] = A[id];
	barrier(CLK_LOCAL_MEM_FENCE);
	for (int i = 1; i < N; i *= 2)
	{
		if (!(idL % (i * 2)) && ((idL + i) < N))
			C[idL] += C[idL + i];
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	C[idL] = C[idL] / 10000.0f;
	if (!idL)
	{
		atomic_add(&B[0], C[idL]);
	}
}

//Kernal to find variance
__kernel void var(__global const int* A, __global int* B, int mean, int initialSize)
{
	int idG = get_global_id(0);

	if (idG < initialSize)
	{
		B[idG] = A[idG] - mean;

		barrier(CLK_LOCAL_MEM_FENCE);

		B[idG] = (B[idG] * B[idG]);
	}
}

//Kernal to find minimun value
__kernel void findMin(__global const int* A, __global int* B, __local int* C)
{
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL] = A[idG];
	barrier(CLK_LOCAL_MEM_FENCE);
	atomic_min(&B[0], C[idL]);
}

//Kernal to find maximum value
__kernel void findMax(__global const int* A, __global int* B, __local int* C)
{
	int idG = get_global_id(0);
	int idL = get_local_id(0);
	C[idL] = A[idG];
	barrier(CLK_LOCAL_MEM_FENCE); 
	atomic_max(&B[0], C[idL]);
}