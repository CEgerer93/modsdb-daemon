#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <momz> <src #> <ensemble>"
    exit 2
fi

momz=$1
src=$2
ensem=$3

sed -e 's/ZZZ/'$momz'/' -e 's/SSS/'$src'/' -e 's/EEE/'$ensem'/' extsdb_mzXXsXX.sh \
    > /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_mz${momz}s${src}.sh

chmod +x /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_mz${momz}s${src}.sh

qsub /data/cpegerer/extraction/ext_scripts/${ensem}/extsdb_mz${momz}s${src}.sh
