#/usr/bin/env python3

import os


def main():
	datafile = "data.csv"
	#create or truncate the file, write headers
	with open(datafile, "w") as f:
		f.write(("Threads, Nodes, Volume, Max Megacalcs, "
				"Ave Megacalcs\n"))
	for threads in [1, 2, 4, 8, 16, 24]:
		for nodes in [100, 500, 1000, 5000]:
			cmd = ("g++ -DNUMTHREADS={} -DNUMNODES={} program1.cpp -o "
					"program1 -lm -fopenmp").format(threads, nodes)
			os.system(cmd)
			cmd = "./program1 {}".format(datafile)
			os.system(cmd)
	cmd = "cat data.csv"
	os.system(cmd)


if __name__ == "__main__":
	main()

