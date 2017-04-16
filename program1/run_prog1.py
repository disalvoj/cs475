#/usr/bin/env python3

import os


def main():
	datafile = "data.csv"
	#create or truncate the file
	open(datafile, "w").close()
	for threads in [1, 2, 4, 8]:
		for nodes in [10, 100, 1000, 5000, 10000, 30000]:
			cmd = ("g++ -DNUMTHREADS={} -DNUMNODES={} program1.cpp -o "
			"program1 -lm -fopenmp").format(threads, nodes)
			os.system(cmd)
			cmd = "./program1 {}".format(datafile)
			os.system(cmd)


if __name__ == "__main__":
	main()

