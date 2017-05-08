#/usr/bin/env python3

# Author: Joshua Kluthe
# Date: 2017.05.01
# Compiles program3.cpp using a variety of thread numbers. For each thread 
# it runs program3.cpp with different amounts of padding to see how the 
# performance changes in relation to cache line location in order to see false 
# sharing in action. It then runs the program using a private variable in the 
# loop to observe the effect of this fix for the false sharing performance hit.

from os import system
from multiprocessing import cpu_count


def main():
	datafile = "data.csv"
	#create or truncate the file
	with open(datafile, "w") as f:
		f.write("Total processors available: {}\n".format(cpu_count()))
	for threads in [1, 2, 4, 6, 8, 10, 12, 14, 16]:
		with open(datafile, "a") as f:
			f.write("{} Threads, Fix #1\n".format(threads))
			f.write("NUMPAD, Peak Mega Adds, Ave Mega Adds\n")
		#compile and run for Fix #1		
		for pad in range(16):
			cmd = ("g++ -DNUMT={} -DFIX={} -DNUMPAD={} program3.cpp -o "
					"program3 -lm -fopenmp").format(threads, 1, pad)
			system(cmd)
			cmd = "./program3 {}".format(datafile)
			system(cmd)
		#compile and run for Fix #2
		with open(datafile, "a") as f:
			f.write("\n{} Threads, Fix #2".format(threads))
			f.write("NUMPAD, Peak Mega Adds, Ave Mega Adds\n")
		cmd = ("g++ -DNUMT={} -DFIX={} -DNUMPAD={} program3.cpp -o "
				"program3 -lm -fopenmp").format(threads, 2, 0)
		system(cmd)
		cmd = "./program3 {}".format(datafile)
		system(cmd)
	#append some newlines for nice formatting just because
	with open(datafile, "a") as f:
		f.write("\n\n\n")
	cmd = "rm -f program3"
	system(cmd)
	cmd = "cat data.csv"
	system(cmd)


if __name__ == "__main__":
	main()


