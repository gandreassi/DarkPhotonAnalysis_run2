import os
import subprocess
import commands

m=0.2
for d in range(0,400):
	m=m+(m*0.01)

	if not d%4==0: continue

	#if d<90 or d>112:
	#	continue
        fname = "dpCard_2017IterV3_"+str(d)+".txt"
	if os.path.isfile(fname):
                os.system("combine -M AsymptoticLimits "+fname+" -m "+str(m)+" -n asympMassIndex_"+str(d)+"")	
	
