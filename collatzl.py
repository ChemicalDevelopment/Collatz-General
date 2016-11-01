#!/usr/bin/python
import time
import sys
import glib

ci = 1
glib.min = int(sys.argv[ci]); ci += 1
glib.ran = int(sys.argv[ci]); ci += 1
glib.max_trials = int(sys.argv[ci]); ci += 1
glib.residue_class = int(sys.argv[ci]); ci += 1

for i in range(0, glib.residue_class):
	glib.mul.append(int(sys.argv[ci])); ci += 1
	glib.add.append(int(sys.argv[ci])); ci += 1
	glib.div.append(int(sys.argv[ci])); ci += 1

glib.print_start_info()

def operf(x, n):
	return (x * glib.mul[n] + glib.add[n]) / glib.div[n]

def f(x):
	return operf(x, x % glib.residue_class)

history = [None] * glib.max_trials

start = time.time() * 1000000

for x in range(glib.min, glib.min + glib.ran):
	trials = 0
	r_x = x
	glib.current_repeated = False
	while trials < glib.max_trials:
		history[trials] = r_x
		r_x = f(r_x)
		for i in range(0, trials+1):
			if (r_x == history[i]):
				glib.current_repeated = True
		trials += 1
		if glib.current_repeated:
			break
	glib.all_repeated = glib.all_repeated and glib.current_repeated
	glib.one_repeated = glib.one_repeated or glib.current_repeated
	if (not glib.current_repeated):
		print "%d does not repeat" % (x)
	else:
		glib.total_repeats += 1
	glib.total_trials += trials

end = time.time() * 1000000

glib.elapsed_micros = end - start

glib.print_end_info()
