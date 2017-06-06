all: omp_data simd_data opencl_data

omp_data: openMPVersion.cpp
	g++ -o omp openMPVersion.cpp -lm -fopenmp
	./omp
	rm omp

simd_data: simd.p5.cpp cpuSIMDVersion.cpp
	g++ -o simd simd.p5.cpp cpuSIMDVersion.cpp -lm -fopenmp
	./simd
	rm simd

opencl_data: gpuOpenCLVersion.cpp
	g++ -o opencl gpuOpenCLVersion.cpp CL/libOpenCL.so -lm -fopenmp
	./opencl
	rm opencl

clean:
	rm *.csv


