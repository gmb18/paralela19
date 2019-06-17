#!/usr/bin/python3

import os, sys, re, statistics

fname = sys.argv[1]
f = open(fname)
lines = f.readlines()
times = [float(re.findall("\d+\.\d+",t)[0]) for t in lines]
mean = sum(times) / len(times)
stdev = statistics.stdev(times)

print(os.path.basename(fname), "- mean:", mean, "stdev:", stdev)
