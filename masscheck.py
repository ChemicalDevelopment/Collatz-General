#!/usr/bin/python

import argparse
import multiprocessing
from multiprocessing import Pool
import subprocess
import time

parser = argparse.ArgumentParser(description='CollatzL mass checker')
parser.add_argument('-min', '--min', default='0', type=str, help='Minimum number to check')
parser.add_argument('-range', '--range', default='25000', type=str, help='Range of numbers to check')
parser.add_argument('-max_trials', '--max_trials', default=1000, type=int, help='Maximum number of trials')
parser.add_argument('-t', '--threads', default=multiprocessing.cpu_count(), type=int, help='Number of threads to run')
parser.add_argument('-pr', '--program', default="c", type=str, help='The program to use')
parser.add_argument('-c', '--congruences', default="2 1 0 2 3 1 1".split(" "), type=str, nargs='+', help='Modular congruences')

args = parser.parse_args()

args.congruences = " ".join(args.congruences)

args.program = args.program.lower()

args.min = int(eval(args.min))
args.range = int(eval(args.range))

if args.program == "c":
    exe = "./collatzl_c.o"
elif args.program == "gmp":
    exe = "./collatzl_gmp.o"

def_cmd = "%s %%d %%d %d %s" % (exe, args.max_trials, args.congruences)

print def_cmd
queue = [None] * args.threads
out = [None] * args.threads

def fmt(id):
    return def_cmd % (args.min + int((id * args.range) / len(queue)), int(args.range / len(queue)))


print "Building program for version: %s" % args.program

buildcmd = subprocess.Popen("make %s" % (args.program), shell=True)
buildcmd.wait()

print "Done"

start = time.time()
for i in range(0, len(queue)):
    print "starting %d th thread" % i
    queue[i] = subprocess.Popen(fmt(i), shell=True, stderr=subprocess.PIPE)


for p in queue:
    out[queue.index(p)] = p.communicate()
    p.wait()
end = time.time()

print "Done with all threads\n"

print "Results: "

for st in out:
    print st[1]

print "Done. Took %f total" % (end - start)