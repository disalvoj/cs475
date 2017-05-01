#/usr/bin/env python3

# Author: Joshua Kluthe
# Date: 2017.05.01
# Compiles program2.cpp using a variety of thread numbers. For each thread 
# it runs program2.cpp with dynamic and static scheduling, and chunk sizes 
# of 1 and 4096.

from os import system
from multiprocessing import cpu_count


def main():
	datafile = "data.csv"
	#create or truncate the file, write headings
	with open(datafile, "w") as f:
		f.write("Total processors available: {}".format(cpu_count()))
		f.write(("\nStatic-1,Static-4096,Dynamic-1,Dynamic-4096,Threads\n"))
	for threads in [1, 2, 4, 6, 8, 10, 12, 14, 16]:
		for sched in ["static", "dynamic"]:
			for chunk in [1, 4096]:
				cmd = ("g++ -DNUMT={} -DSCHED={} -DCHUNK={} program2.cpp -o "
						"program2 -lm -fopenmp").format(threads, sched, chunk)
				system(cmd)
				cmd = "./program2 {}".format(datafile)
				system(cmd)
		#program.cpp will write the result of it's run, but here we must
		#append the number of threads and add a newline
		with open(datafile, "a") as f:
			f.write("{}\n".format(threads))
	cmd = "rm -f program2"
	system(cmd)
	cmd = "cat data.csv"
	system(cmd)


if __name__ == "__main__":
	main()

