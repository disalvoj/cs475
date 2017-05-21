#/usr/bin/env python3


"""
Author: Joshua Kluthe
Date: 2017.05.21

This script compiles and runs program5.cpp using a variety of array sizes and 
for two different types of tests, which are passed as #defines to the compiler.
It also creates/truncates a file "data.csv" which is passed as an argument to 
program5 and is used to record the data from the benchmarks. A second file, 
"speedup_data.csv", is used to store only the speedup data to simplify graphing.
"""


from os import system
from multiprocessing import cpu_count


def main():
    datafile = "data.csv"
    speedup_data = "speedup_data.csv"
    #create or truncate the files
    with open(datafile, "w") as f:
        f.write("Total processors available: {}\n".format(cpu_count()))
    with open(speedup_data, "w") as f:
        f.write("Speedup Data (C++ Speed / SIMD SSE Speed)")
    start = 1000
    end = 32000000
    tests = ["SIMD SSE Multiplication vs C++ Multiplication",
        "SIMD SSE Multiplication/Reduction vs C++ Multiplication/Reduction"]
    for test in tests:
        with open(datafile, "a") as f:
            f.write("\nTesting {}\n".format(test))
        with open(speedup_data, "a") as f:
            f.write("{}\n".format(test))
        for size in list(range(start, end, int((end - start)/20))) + [end]:
            cmd = ("g++ -DARRAY_SIZE={} "
                "-DTEST={} program5.cpp simd.p5.cpp -o program5 -lm "
                "-fopenmp").format(size, tests.index(test))
            system(cmd)
            cmd = "./program5 {} {}".format(datafile, speedup_data)
            system(cmd)
    #append some newlines for nice formatting just because
    with open(datafile, "a") as f:
        f.write("\n\n\n")
    cmd = "rm -f program5"
    system(cmd)
    cmd = "cat data.csv"
    system(cmd)


if __name__ == "__main__":
    main()


