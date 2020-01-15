#!/bin/bash
#PBS -N ext
#PBS -l nodes=1:ppn=24
#PBS -l walltime=48:00:00


ml=MLML
ms=MSMS
src=SSS
momz=ZZZ
xi=-4
xf=4
yi=-4
yf=4
zi=-8
zf=8

# ensemble=cl21_32_96_b6p1_m0p2800_m0p2450
ensemble=EEE
beta="b6p1"
Nx=32
Nt=96

# Variables for the freefield extraction
numCCM=2 



OUTDIR="/data/cpegerer/SDB_pion/${beta}/${ensemble}/pdf_m${ml}_db/freefield"
# Make the output directory tree
mkdir -p ${OUTDIR}/pz${momz}/${src}/pion_2pt
mkdir -p ${OUTDIR}/pz${momz}/${src}/hxyz



export OMP_NUM_THREADS=24

# export I_MPI_PIN=1
# export I_MPI_PIN_DOMAIN=core
# export I_MPI_FABRICS=shm:tmi
# export KMP_AFFINITY=verbose,compact,granularity=thread
# export HFI_NO_CPUAFFINITY=1

# mpirank=48
# mpiproc=`expr $PBS_NUM_NODES \* 2`

# EXECUTABLE=${PBS_O_WORKDIR}/modsdb_daemon
# ARGS="runxmls/daemon_${PBS_JOBID}.ini.xml"

cd $PBS_O_WORKDIR
echo "Working in $PBS_O_WORKDIR"

# source the environment
source ${HOME}/qcd_software/cyclops/scalar/env.sh

# Use a python interface to generate the xml
pyinf=${HOME}/structure/special_keys

$pyinf/write_modsdb_daemon_multikey_free.py -b $beta -e $ensemble -X $Nx -T $Nt -m $ml \
    -s $src -p $momz -q $momz -x $xi -u $xf -y $yi -v $yf -z $zi -w $zf \
    -c $numCCM > /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml


echo "Starting the extraction"
# Do all the extractions here
${HOME}/pionPDF/extraction/free/modsdb_daemon_free /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml \
    &>> /data/cpegerer/extraction/EXT_free_${momz}_${src}_ml${ml}_ms${ms}


exit 0
