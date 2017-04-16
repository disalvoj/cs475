#/usr/bin/env python3

# Author: Joshua Kluthe
# Date: 2017.04.17
# Compiles program1.cpp using a variety of thread and node numbers, runs
# the resulting executable, and cats the collected data to terminal.

from os import system
from multiprocessing import cpu_count


def main():
	datafile = "data.csv"
	#create or truncate the file, write headers
	with open(datafile, "w") as f:
		f.write("Total processors available: {}".format(cpu_count()))
		#cmd = "nproc > data.csv"
		#os.system(cmd)
		f.write(("\nThreads, Nodes, Volume, Max Megacalcs, "
				"Ave Megacalcs\n"))
	for threads in [1, 2, 4, 8, 16, 24]:
		for nodes in [100, 500, 1000, 5000]:
			cmd = ("g++ -DNUMTHREADS={} -DNUMNODES={} program1.cpp -o "
					"program1 -lm -fopenmp").format(threads, nodes)
			system(cmd)
			cmd = "./program1 {}".format(datafile)
			system(cmd)
	cmd = "rm -f program1"
	system(cmd)
	cmd = "cat data.csv"
	system(cmd)


if __name__ == "__main__":
	main()

