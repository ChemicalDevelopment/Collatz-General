min = 0
ran = 1000
max_trials = 1000

residue_class = 2

mul = []
add = []
div = []

total_trials = 0; total_repeats = 0

all_repeated = True; one_repeated = False; current_repeated = False

elapsed_micros = 0

def print_start_info():
	print "Running CollatzLpy v0.0.1"
	print "Testing from %d to %d, going to %d max trials\n" % (min, min + ran, max_trials)

	def get_rel(i):
		st = ""
		if div[i] != 1 and add[i] != 0: st += "("
		if mul[i] != 1: st += "%d" % (mul[i])
		st += "x"
		if add[i] != 0: st += "+%d" % (add[i])
		if div[i] != 1 and add[i] != 0: st += ")"
		if div[i] != 1: st += "/%d" % (div[i])
		return st
		
	for i in range(0, residue_class):
		print "If x %% %d = %d, then f(x) = %s" % (residue_class, i, get_rel(i))

	print "Does the iteration of f(f(f(f(f...(x))))) reach a cycle?\n"

def print_time_info():
	elapsed_ms = elapsed_micros/1000.; elapsed_s = elapsed_ms/1000.;elapsed_m=elapsed_s/60.
	elapsed_hr=elapsed_m/60.;elapsed_days=elapsed_hr/24.
	print "\nTook"
	if elapsed_micros < 600000000.0: print "    %f micros" % (elapsed_micros)
	print "    %f ms" % (elapsed_ms)
	if elapsed_s > .5 and elapsed_s < 120.: print "    %f s" % (elapsed_s)
	if elapsed_m > .5 and elapsed_m < 120.: print "    %f m" % (elapsed_m)
	if elapsed_hr > .5 and elapsed_hr < 72.: print "    %f hr" % (elapsed_hr)
	if elapsed_days > .5: print "    %f days" % (elapsed_days)

def print_end_info():
	if (all_repeated):
		print "All tested inputs repeat (%d total)" % (total_repeats)
	else:
		print "%d numbers repeated" % (total_repeats)
	print_time_info()
	print "\nRan a total of %d trials" % total_trials
	print "    Average trials per microsecond : %f" % (total_trials/elapsed_micros)
	print "    Average numbers checked per microseconds : %f" % (ran/elapsed_micros)
	print "    Average trials per number : %f" % ((total_trials+0.0)/ran)

	