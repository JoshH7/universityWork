
#include <vector>
#include <iostream>
#include <iomanip> 
#include <helper_cuda.h>
#include <cuda.h>
#include <cuda_runtime_api.h>



#include "Utils.h"


/// This function uses an ifstream to read the file, and stores it into a string.
/// The string is then parsed to get the 6th data column and return it

vector<int>* cData(std::string filename)
{
	vector<int>* data = new vector<int>;
	ifstream file(filename);
	string S;
	int N  = 0;
	while (std::getline(file, S))
	{
		std::string T ;
		for (int i = 0; i < S.size(); i++)
		{
			if (N  < 5)
			{
				if (S[i] == ' ')
				{
					N ++;
				}
			}
			else
			{
				T  += S[i];
			}
		}
		data->push_back(std::stof(T ) * 100);
		N  = 0;
	}
	return data;
}

int main(int argc, char** argv)
{
	int P  = 0;
	int D  = 0;
	typedef int intType1;
	// Store the filename and it's absolute path separately and append them after to read the file.
	// This allows the filename to be displayed without the entire path, and allows either to be changed without the other being affected
	std::string file = "C:\Users\earth\Desktop\ParallelP\Assignment\Assignment";
	std::string weatherData = "temp_lincolnshire.txt";
	file.append(weatherData);


	vector<int>* data = cData(file);
	std::cout << "Reading file complete" << std::endl;
	int DS  = data->size();
	try
	{
		cl::Context context = GetContext(P , D );
		std::cout << "Running on " << GetPlatformName(P ) << ", " << GetDeviceName(P , D ) << std::endl;
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);
		cl::Program::Sources sources;
		AddSources(sources, "kernels.cu");
		cl::Program program(context, sources);
		try
		{
			program.build();
		}
		catch (const cl::Error& err)
		{
			std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}


		//Part 4 - memory allocation

		//the following part adjusts the length of the input vector so it can be run for a specific workgroup size
		//if the total input length is divisible by the workgroup size
		//this makes the code more efficient
		size_t local_size = 1024;

		size_t padding_size = data->size() % local_size;

		//if the input vector is not a multiple of the local_size
		//insert additional neutral elements (0 for addition) so that the total will not be affected
		if (padding_size)
		{
			//create an extra vector with neutral values
			std::vector<int> A_ext(local_size - padding_size, 0);
			//append that extra vector to our input
			data->insert(data->end(), A_ext.begin(), A_ext.end());
		}

#pragma region Kernel Buffers

		size_t input_elements = data->size(); // Number of input elements
		size_t input_size = data->size() * sizeof(intType1); // Size in bytes
		size_t nr_groups = input_elements / local_size;

		// Host - Output vectors
		size_t output_size = input_elements * sizeof(intType1); // Size in bytes

		vector<intType1> B(input_elements);
		vector<intType1> C(input_elements);
		vector<intType1> D(input_elements);
		vector<intType1> E(input_elements);
		vector<unsigned int> F(input_elements);
		vector<intType1> G(input_elements);
		vector<intType1> H(input_elements);


		// Device - Buffers  |  One input buffer and several output buffers
		cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);

		cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_D(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_E(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_F(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_G(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_H(context, CL_MEM_READ_WRITE, output_size);

#pragma endregion

#pragma region Enqueue buffers + Create kernels

		// Copy array A to and initialise other arrays on device memory
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &(*data)[0]);

		// Zero buffer on device memory for output
		queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_C, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_D, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_E, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_F, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_G, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_H, 0, 0, output_size);

		// Setup and execute all kernels (i.e. device code)
		cl::Kernel kernel_1 = cl::Kernel(program, "reduce_find_min");
		kernel_1.setArg(0, buffer_A);
		kernel_1.setArg(1, buffer_B);
		kernel_1.setArg(2, cl::Local(local_size * sizeof(intType1)));//local memory size

		cl::Kernel kernel_2 = cl::Kernel(program, "reduce_find_max");
		kernel_2.setArg(0, buffer_A);
		kernel_2.setArg(1, buffer_C);
		kernel_2.setArg(2, cl::Local(local_size * sizeof(intType1)));

		// ======================== Atomic Kernels ======================== //
		cl::Kernel kernel_1A = cl::Kernel(program, "at_find_min");
		kernel_1A.setArg(0, buffer_A);
		kernel_1A.setArg(1, buffer_G);
		kernel_1A.setArg(2, cl::Local(local_size * sizeof(intType1)));

		cl::Kernel kernel_2A = cl::Kernel(program, "at_find_max");
		kernel_2A.setArg(0, buffer_A);
		kernel_2A.setArg(1, buffer_H);
		kernel_2A.setArg(2, cl::Local(local_size * sizeof(intType1)));
		// ======================== [END] Atomic Kernels ======================== //

		cl::Kernel kernel_3 = cl::Kernel(program, "reduce_find_sum");
		kernel_3.setArg(0, buffer_A);
		kernel_3.setArg(1, buffer_D);
		kernel_3.setArg(2, cl::Local(local_size * sizeof(intType1)));

#pragma endregion

#pragma region Profile Events + Call Kernels

		// Create the profiling events that will measure the time each kernel takes to run
		// 1A & 2A are the Atomic versions min/max, which are profile_event 1 & 2 respectively
		cl::Event prof_event1;
		cl::Event prof_event1A;
		cl::Event prof_event2;
		cl::Event prof_event2A;
		cl::Event prof_event3;
		cl::Event prof_event4; // These are for the variance kernel and the summing of that set of results
		cl::Event prof_event5; // They need to be combined to give the total time to the variance result

		// Call all the kernels in sequence - Except for the mean and standard deviation which require results from these to work
		queue.enqueueNDRangeKernel(kernel_1, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event1);
		queue.enqueueNDRangeKernel(kernel_2, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event2);
		queue.enqueueNDRangeKernel(kernel_3, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event3);
		queue.enqueueNDRangeKernel(kernel_1A, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event1A);
		queue.enqueueNDRangeKernel(kernel_2A, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event2A);

#pragma endregion

#pragma region Read Buffers
		// ================ Copy the result from device to host ================
		// Also stop the profile timers and save the values here

		// Reduce Min
		queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0]);
		uint64_t p1 = prof_event1.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event1.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// Reduce Max
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, output_size, &C[0]);
		uint64_t p2 = prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// Mean
		queue.enqueueReadBuffer(buffer_D, CL_TRUE, 0, output_size, &D[0]);
		uint64_t p3 = prof_event3.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event3.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// Reduce Min
		queue.enqueueReadBuffer(buffer_G, CL_TRUE, 0, output_size, &G[0]); // For the atomic version
		uint64_t p1A = prof_event1A.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event1A.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// Atomic Max
		queue.enqueueReadBuffer(buffer_H, CL_TRUE, 0, output_size, &H[0]);
		uint64_t p2A = prof_event2A.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2A.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		////queue.enqueueReadBuffer(buffer_I, CL_TRUE, 0, output_size, &I[0]);

#pragma endregion

		// ===================== Kernel Results =====================

		float minVal = (float)B[0] / 100.0f;
		float maxVal = (float)C[0] / 100.0f;
		float atomMinVal = (float)G[0] / 100.0f;
		float atomMaxVal = (float)H[0] / 100.0f;
		float mean = ((float)D[0] / data->size()) / 100.0f;

		// ===================== [END] Kernel Results =====================

#pragma region Variance + Std Dev

		// This section calculates the variance and from that the standard deviation
		// 1st create kernel_4 and give it the dataset.
		// 2nd take the returned squared values from kernel_4 in buffer E and pass them to kernel_5 for summing

		// Create new kernel. Initial size is to allow for padding protection on the X - mean calculation
		cl::Kernel kernel_4 = cl::Kernel(program, "find_variance");
		kernel_4.setArg(0, buffer_A);
		kernel_4.setArg(1, buffer_E);
		kernel_4.setArg(2, (int)(mean * 100));
		kernel_4.setArg(3, DS );

		// Call the kernel, then read the return buffer for results.
		// Stop profile timer and save the value as well
		queue.enqueueNDRangeKernel(kernel_4, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event4);
		queue.enqueueReadBuffer(buffer_E, CL_TRUE, 0, output_size, &E[0]);
		uint64_t p4 = prof_event4.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event4.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// Create new sum kernel. This sum divides by 10k to account for the exponential growth of 100^2
		cl::Kernel kernel_5 = cl::Kernel(program, "reduce_find_sum_variance");
		kernel_5.setArg(0, buffer_E);
		kernel_5.setArg(1, buffer_F);
		kernel_5.setArg(2, cl::Local(local_size * sizeof(intType1)));

		// Call the kernel and read the return buffer
		queue.enqueueNDRangeKernel(kernel_5, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &prof_event5);
		queue.enqueueReadBuffer(buffer_F, CL_TRUE, 0, output_size, &F[0]);
		uint64_t p5 = prof_event5.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event5.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		// ========== Results ==========
		float variance = (float)F[0] / F.size();
		float stdev = sqrt(variance);


#pragma region Console Output
		// ================================== Printing Details ================================== //
		std::cout << "\n\n##========================== Details ==========================##\n" << std::endl;
		std::cout << "Weather data file: " << weatherData << std::endl;
		std::cout << "Total data values: " << (*data).size() << std::endl;
		std::cout << "Read file run time: " << (readTime / 1000.0f) << " seconds" << std::endl;
		std::cout << "Total run time: " << (totalTime / 1000.0f) << " seconds" << std::endl;

		// ================================== Printing results ================================== //
		std::cout << "\n\n##========================== Results ==========================##\n" << std::endl;

		std::cout << "Reduce Min = " << minVal << "	|	Execution Time [ns]: " << p1 << std::endl;
		std::cout << "Atomic Min = " << atomMinVal << "	|	Execution Time [ns]: " << p1A << std::endl;

		std::cout << "\nReduce Max = " << maxVal << "		|	Execution Time [ns]: " << p2 << std::endl;
		std::cout << "Atomic Max = " << maxVal << "		|	Execution Time [ns]: " << p2A << std::endl;

		std::cout << "\nMean = " << std::fixed << std::setprecision(2) << mean << "		|	Execution Time [ns]: " << p3 << std::endl;

		std::cout << "\nVariance = " << std::fixed << std::setprecision(2) << variance << "	|	Execution Time [ns]: " << (p4 + p5) << std::endl;
		std::cout << "\nStandard Deviation = " << std::fixed << std::setprecision(2) << stdev << std::endl;

		//std::cout << "\n\nSort: " << I[0] << "  -  " << I[DS  - 1] << std::endl;

		// ================================== Printing Profiling Data ================================== //
		std::cout << "\n\n##========================== Profiling Data ==========================##\n" << std::endl;

		std::cout << "Reduce Min	= " << GetFullProfilingInfo(prof_event1, ProfilingResolution::PROF_US) << endl;
		std::cout << "Atomic Min	= " << GetFullProfilingInfo(prof_event1A, ProfilingResolution::PROF_US) << endl;

		std::cout << "\nReduce Max	= " << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << endl;
		std::cout << "Atomic Max	= " << GetFullProfilingInfo(prof_event2A, ProfilingResolution::PROF_US) << endl;

		std::cout << "\nMean		= " << GetFullProfilingInfo(prof_event3, ProfilingResolution::PROF_US) << endl;
		std::cout << "Variance	= " << GetFullProfilingInfo(prof_event4, ProfilingResolution::PROF_US) << endl;
		std::cout << "\n" << endl;

#pragma endregion

		std::system("pause");

	}
	catch (cl::Error err)
	{
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
	}

	return 0;
}