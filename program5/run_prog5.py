#/usr/bin/env python3


# Author: Joshua Kluthe
# Date: 2017.05.21
"""
This script compiles and runs program5.cpp using a variety of array sizes and 
for two different types of tests, which are passed as #defines to the compiler.
It also creates/truncates a file "data.csv" which is passed as an argument to 
program5 and is used to record the data from the benchmarks.
"""


from os import system
from multiprocessing import cpu_count


def main():
    datafile = "data.csv"
    #create or truncate the file
    with open(datafile, "w") as f:
        f.write("Total processors available: {}\n".format(cpu_count()))
    start = 1000
    end = 32000000
    tests = ["SIMD SSE Multiplication vs C++ Multiplication",
        "SIMD SSE Mulitpication/Reduction vs C++ Multiplication/Reduction"]
    for test in tests:
        for size in list(range(start, end, int((end - start)/20))) + [end]:
            with open(datafile, "a") as f:
                f.write("\nTesting {}\n".format(test))
            cmd = ("/usr/local/common/gcc-5.4.0/bin/g++ -DARRAY_SIZE={} "
                "-DTEST={} program5.cpp simd.p5.cpp -o program5 -lm "
                "-fopenmp").format(size, tests.index(test))
            system(cmd)
            cmd = "./program5 {}".format(datafile)
            system(cmd)
            cmd = "rm -f program5"
            system(cmd)
#            with open(datafile, "a") as f:
#                f.write("\nDone Testing.\n")
    #append some newlines for nice formatting just because
    with open(datafile, "a") as f:
        f.write("\n\n\n")
    cmd = "cat data.csv"
    system(cmd)


if __name__ == "__main__":
    main()


