#!/bin/bash

cd $3
echo $1
echo $2
echo $3
pwd
echo "hello"
eval `scramv1 runtime -sh`

root -l -b -q trimscoutMakeTheCards.C\(\"$1\"\,\"$2\"\,false\)
