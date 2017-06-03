#!/usr/bin/env python3

from os import system


def main():
    mult_data = "mult_data.csv"
    with open(mult_data, "w") as f:
        f.write("Local Size,Global Size,Performance (Giga)\n")
    mult_add_data = "mult_add_data.csv"
    with open(mult_add_data, "w") as f:
        f.write("Local Size,Global Size,Performance (Giga)\n")
    mult_reduce_data = "mult_reduce_data.csv"
    with open(mult_reduce_data, "w") as f:
        f.write("Local Size,Global Size,Performance (Giga)\n")
    for local_size in list(range(8, 513, 24)):
        for global_size in list(range(1024, 8389633, 262144)):
            cmd = ("g++ -DLOCAL_SIZE={} -DGLOBAL_SIZE={} -o mult mult_and_add.cpp "
                "CL/libOpenCL.so -lm -fopenmp").format(local_size, global_size)
            system(cmd)
            cmd = "./mult {} mult.cl".format(mult_data)
            system(cmd)
            cmd = ("g++ -DLOCAL_SIZE={} -DGLOBAL_SIZE={} -o mult_add mult_and_add.cpp "
                "CL/libOpenCL.so -lm -fopenmp").format(local_size, global_size)
            system(cmd)
            cmd = "./mult_add {} mult_and_add.cl".format(mult_add_data)
            system(cmd)
    for local_size in list(range(32, 257, 32)):
        for global_size in list(range(1024, 20389633, 262144)):
            cmd = ("g++ -DLOCAL_SIZE={} -DGLOBAL_SIZE={} -o mult_reduce mult_and_reduce.cpp "
                "CL/libOpenCL.so -lm -fopenmp").format(local_size, global_size)
            system(cmd)
            cmd = "./mult_reduce {} mult_and_reduce.cl".format(mult_reduce_data)
            system(cmd)
    cmd = "rm -f mult mult_add mult_reduce"
    system(cmd)


if __name__ == "__main__":
    main()


