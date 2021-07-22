#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS
#include <iostream>
#include <vector>
#include <iomanip>
#include "Utils.h"
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
// Function takes the fileLocation and uses ifstream to read in the contents of that file.
vector<int>* srData(std::string filename)
{
	string s1;
	int s2 = 1;
	vector<int>* rData = new vector<int>;
	ifstream rFile(filename);
	while (std::getline(rFile, s1))
	//loop to read file, stopping at the 5th point and breaking
	{
		std::string s3;
		for (int i = 1; i < s1.size(); i++)
		{
			if (s2 < 6)
			{
				if (s1[i] == ' ')
				{
					s2++;
				}
			}
			else
			{
				s3 += s1[i];
			}
		}
		rData->push_back(std::stof(s3) * 100);
		s2 = 1;
	}
	return rData;
}

//Main solution, most of the buffer and kernal management takes place here
int solution(int argc, char** argv)
{
	typedef int myInt1;
	int pID = 0;
	int dID = 0;
	// Find and store filename and filepath and appending it to a DB file
	std::string fileLocation = "C:\Users\earth\Desktop\ParallelP\Assignment";
	std::string dataFile = "temp_lincolnshire.txt";
	fileLocation.append(dataFile);
	vector<int>* rData = srData(fileLocation);

	std::cout << "Data finished reading" << std::endl;
	int sSize = rData->size();
	try
	{
		cl::Context context = GetContext(pID, dID);
		std::cout << "The platform it is running on is " << GetPlatformName(pID) << std::endl;
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);
		cl::Program::Sources sources;
		AddSources(sources, "kernel.cl");
		cl::Program program(context, sources);
		try
		{
			program.build();
		}
		catch (const cl::Eor& E)
		{
			throw E;
		}
		size_t size = 1024;
		size_t size2 = rData->size() % size;
		if (size2)
		{
			std::vector<int> xT(size - size2, 0);
			rData->insert(rData->end(), xT.begin(), xT.end());
		}

    //Building buffers, which act as ways for the main.cpp file to communicate with kernels
#pragma region buffers
		//Buffer layout for this project is seven output buffers and one input buffer
		size_t inD = rData->size();
		size_t inS = rData->size() * sizeof(myInt1);
		size_t group = inD / size;
		size_t outS = inD * sizeof(myInt1);
		vector<myInt1> B(inD);
		vector<myInt1> C(inD);
		vector<myInt1> D(inD);
		vector<myInt1> E(inD);
		vector<myInt1> F(inD);
		vector<myInt1> G(inD);
		vector<myInt1> H(inD);
		cl::buffer1(context, CL_MEM_READ_ONLY, inS);
		cl::buffer2(context, CL_MEM_READ_WRITE, outS);
		cl::buffer3(context, CL_MEM_READ_WRITE, outS);
		cl::buffer4(context, CL_MEM_READ_WRITE, outS);
		cl::buffer5(context, CL_MEM_READ_WRITE, outS);
		cl::buffer6(context, CL_MEM_READ_WRITE, outS);
		cl::buffer7(context, CL_MEM_READ_WRITE, outS);
		cl::buffer8(context, CL_MEM_READ_WRITE, outS);
#pragma endregion

    //Assigning buffers to kernels
#pragma region buffersKernels
		queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, inS, &(*rData)[0]);
		queue.enqueueFillBuffer(bufferB, 0, 0, outS);
		queue.enqueueFillBuffer(bufferC, 0, 0, outS);
		queue.enqueueFillBuffer(bufferD, 0, 0, outS);
		queue.enqueueFillBuffer(bufferE, 0, 0, outS);
		queue.enqueueFillBuffer(bufferF, 0, 0, outS);
		queue.enqueueFillBuffer(bufferG, 0, 0, outS);
		queue.enqueueFillBuffer(bufferH, 0, 0, outS);

		cl::Kernel K1 = cl::Kernel(program, "reduceMethodMax");
		K1.setArg(0, bufferA);
		K1.setArg(1, bufferB);
		K1.setArg(2, cl::Local(size * sizeof(myInt1)));

		cl::Kernel K2 = cl::Kernel(program, "reduceMethodMin");
		K2.setArg(0, bufferA);
		K2.setArg(1, bufferC);
		K2.setArg(2, cl::Local(size * sizeof(myInt1)));

		cl::Kernel K3 = cl::Kernel(program, "reduceMethodSum");
		K3.setArg(0, bufferA);
		K3.setArg(1, bufferD);
		K3.setArg(2, cl::Local(size * sizeof(myInt1)));

		cl::Kernel K1A = cl::Kernel(program, "findMin");
		K1A.setArg(0, bufferA);
		K1A.setArg(1, bufferG);
		K1A.setArg(2, cl::Local(size * sizeof(myInt1)));

		cl::Kernel K2A = cl::Kernel(program, "findMax");
		K2A.setArg(0, bufferA);
		K2A.setArg(1, bufferH);
		K2A.setArg(2, cl::Local(size * sizeof(myInt1)));

#pragma endregion
	//Calling kernels
#pragma region eventsKernels

		cl::Event eventK1;
		cl::Event eventK1A;
		cl::Event eventK2;
		cl::Event eventK2A;
		cl::Event eventK3;
		cl::Event eventK4;
		cl::Event eventK5;
		// Call all kernels apart from mean and standard deviation, as I need the data from these for those to work
		queue.enqueueNDRangeKernel(K1, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK1);
		queue.enqueueNDRangeKernel(K1A, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK1A);
		queue.enqueueNDRangeKernel(K2, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK2);
		queue.enqueueNDRangeKernel(K2A, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK2A);
		queue.enqueueNDRangeKernel(K3, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK3);
#pragma endregion
		//Reading buffer data
#pragma region readBuffers
		//Max
		queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, outS, &C[0]);
		uint64_t pr1 = eventK2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK2.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		//Min
		queue.enqueueReadBuffer(bufferB, CL_TRUE, 0, outS, &B[0]);
		uint64_t pr2 = eventK1.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK1.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		//Mean
		queue.enqueueReadBuffer(bufferD, CL_TRUE, 0, outS, &D[0]);
		uint64_t pr3 = eventK3.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK3.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		//Atomic Max
		queue.enqueueReadBuffer(bufferH, CL_TRUE, 0, outS, &H[0]);
		uint64_t pr1A = eventK2A.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK2A.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		//Atomic Min
		queue.enqueueReadBuffer(bufferG, CL_TRUE, 0, outS, &G[0]); 
		uint64_t pr2A = eventK1A.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK1A.getProfilingInfo<CL_PROFILING_COMMAND_START>();
#pragma endregion
		//Storing data from kernels into floats
		float minV = (float)B[0] / 100.0f;
		float maxV = (float)C[0] / 100.0f;
		float AMinV = (float)G[0] / 100.0f;
		float AMaxV = (float)H[0] / 100.0f;
		float meanV = ((float)D[0] / rData->size()) / 100.0f;
		//Reading Variance and STD kernels
#pragma region VarSTD
		cl::Kernel K4 = cl::Kernel(program, "var");
		K4.setArg(0, bufferA);
		K4.setArg(1, bufferE);
		K4.setArg(2, (int)(meanV * 100));
		K4.setArg(3, sSize);
		queue.enqueueNDRangeKernel(K4, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK4);
		queue.enqueueReadBuffer(bufferE, CL_TRUE, 0, outS, &E[0]);
		uint64_t pr4 = eventK4.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK4.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		cl::Kernel kernel_5 = cl::Kernel(program, "reduceFindSumVar");
		kernel_5.setArg(0, bufferE);
		kernel_5.setArg(1, bufferF);
		kernel_5.setArg(2, cl::Local(size * sizeof(myInt1)));

		queue.enqueueNDRangeKernel(kernel_5, cl::NullRange, cl::NDRange(inD), cl::NDRange(size), NULL, &eventK5);
		queue.enqueueReadBuffer(bufferF, CL_TRUE, 0, outS, &F[0]);
		uint64_t pr5 = eventK5.getProfilingInfo<CL_PROFILING_COMMAND_END>() - eventK5.getProfilingInfo<CL_PROFILING_COMMAND_START>();

		float var = (float)F[0] / F.size();
		float stde = sqrt(var);

		//Summery of dataset as output
#pragma region Output
		std::cout << "Weather data files" << dataFile << std::endl;
		std::cout << "Total data summery" << (*rData).size() << std::endl;
		std::cout << "Max" << maxV << pr1 << std::endl;
		std::cout << "\nReduce Max	= " << GetFullProfilingInfo(eventK2, ProfilingResolution::PROF_US) << endl;
		std::cout << "Atomic Max" << maxV << pr1A << std::endl;
		std::cout << "Atomic Max" << GetFullProfilingInfo(eventK2A, ProfilingResolution::PROF_US) << endl;
		std::cout << "Reduce Min" << minV << pr2 << std::endl;
		std::cout << "Reduce Min" << GetFullProfilingInfo(eventK1, ProfilingResolution::PROF_US) << endl;
		std::cout << "Atomic Min " << AMinV << pr2A << std::endl;
		std::cout << "Atomic Min" << GetFullProfilingInfo(eventK1A, ProfilingResolution::PROF_US) << endl;
		std::cout << "\nMean " << std::fixed << std::setprecision(2) << meanV <<  pr3 << std::endl;
		std::cout << "\nMean" << GetFullProfilingInfo(eventK3, ProfilingResolution::PROF_US) << endl;
		std::cout << "\nVariance" << std::fixed << std::setprecision(2) << var << (pr4 + pr5) << std::endl;
		std::cout << "Variance" << GetFullProfilingInfo(eventK4, ProfilingResolution::PROF_US) << endl;
		std::cout << "\nStandard Deviation" << std::fixed << std::setprecision(2) << stde << std::endl;;
		std::cout << "\n" << endl;
#pragma endregion
		//End 
		std::system("End");
	}
	//Error Catch
	catch (cl::Eor E)
	{
		std::cE << "EOR: " << E.what() << ", " << getEorString(E.E()) << std::endl;
	}
	return 0;
}