

omp_data: openMPVersion.cpp
	g++ -o omp openMPVersion.cpp -lm -fopenmp
	./omp
	rm omp

simd_data: simd.p5.cpp cpuSIMDVersion.cpp
	g++ -o simd simd.p5.cpp cpuSIMDVersion.cpp -lm -fopenmp
	./simd
	rm simd

