#!/bin/bash

#############
#
# LOOP OVER SRC AND CONFIGS AND REMAPS MOD FILES INTO CORRECT FORMAT
#
#############

if [ $# -ne 4 ]; then
    echo "Usage: $0 <4pt byte count> <2pt byte count> <# 4pt keys> <# 2pt keys>"
    exit 1
fi

bytes4pt=$1
bytes2pt=$2
keys4pt=$3
keys2pt=$4

# Source the environment
source ${HOME}/qcd_software/cyclops/scalar/env.sh
# Grab the salvage reader
SALV=${HOME}/pionPDF/extraction/salvage/salvage_reader 


echo "Working in `pwd`"


for src in 5; do
    cd ${src}/

    for cnfg in `seq 1000 10 6020`; do

	# Only do the salvaging if configuration directory was found
	if [ -d $cnfg ]; then

	    cd ${cnfg}/

	    mkdir -p ../../../${src}/${cnfg}/

	    # Do the reformatting
	    $SALV pion_PDF_4pt.mod $bytes4pt $keys4pt pion_PDF_2pt.mod $bytes2pt $keys2pt
	
            # Move reformated mod files to their proper homes
	    mv pion_PDF_2pt_remade.mod ../../../${src}/${cnfg}/pion_PDF_2pt.mod
	    mv pion_PDF_4pt_remade.mod ../../../${src}/${cnfg}/pion_PDF_4pt.mod

	    cd ../

	else
	    echo "Skipping $cnfg, did not find..."
	fi

    done

    # Back to sources
    cd ../
done

exit 0
