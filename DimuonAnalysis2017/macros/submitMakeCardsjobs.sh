#!/bin/bash

workDir=`pwd`
script=jobsMakeCards.sh

if [ -z "$1" ]
  then
    echo "No file directory given"
    exit 1
fi

files=`ls $1/scout*`

echo $workDir
#echo $files
suffix='Xscout_hists_'

for file in $files; do
	#echo $file	
	outputFile=${file/scout_/$suffix}
	bsub -o out.%J -q 8nh  $script ${file} ${outputFile} ${workDir}
done
