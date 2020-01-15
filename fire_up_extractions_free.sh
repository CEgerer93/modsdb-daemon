#!/bin/bash

if [ $# -ne 5 ]; then
    echo "Usage: $0 <src #> <ensemble> <mom> <ml> <ms>"
    exit 2
fi

src=$1
ensem=$2
mom=$3
ml=$4
ms=$5

sed -e 's/ZZZ/'$mom'/' -e 's/SSS/'$src'/' -e 's/EEE/'$ensem'/' -e 's/MLML/'$ml'/' -e 's/MSMS/'$ms'/' \
    extsdb_sXX_free.sh > /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_free_pz${mom}_s${src}_ml${ml}_ms${ms}.sh

chmod +x /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_free_pz${mom}_s${src}_ml${ml}_ms${ms}.sh

qsub /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_free_pz${mom}_s${src}_ml${ml}_ms${ms}.sh
