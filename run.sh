ulimit -c unlimited

#!/bin/bash
op=$1
if [ "$op" == "start" ]; then

  ./mcq --dfg_file=data/test.txt --II=2  --pea_column=5 --pea_row=4
 
elif [ "$op" == "clear" ]; then
  rm -rf log/*
else
  echo "./run (start | clear)"
fi
