import os
import subprocess
import commands
import re
from glob import glob
import sys
from shutil import move
test = True

year = sys.argv[1]
os.makedirs("combine_output/"+year)

files = glob("output/dpCard_"+year+"IterV3_*.txt")
for fname in files:
	m = re.search(r"_m(\d+\.?\d+)_", fname).group(1)
	d = re.search(r"_(\d+).txt", fname).group(1)
	print "ID {0},  m={1}".format(d,m)
	if test and (not int(d)%10==0): continue
	if os.path.isfile(fname):
                os.system("combine -M AsymptoticLimits "+fname+" -m "+m+" -n asympMassIndex_"+d)	

outfiles = glob("higgsCombineasympMassIndex_*.root")
for of in outfiles:	
	move(of, "combine_output/"+year+"/")
