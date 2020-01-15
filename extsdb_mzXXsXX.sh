#!/bin/bash
#PBS -N ext
#PBS -l nodes=1:ppn=24
#PBS -l walltime=24:00:00


ml=-0.2350
momz=ZZZ
src=SSS
xi=0
xf=0
yi=0
yf=0
zi=-8
zf=8

# ensemble=cl21_32_96_b6p1_m0p2800_m0p2450
ensemble=EEE
beta="b6p3"
Nx=32
Nt=64

# # Some variables for the b6p1 32x96 lattices
# numCCM=4 # Runs with norm(momz)<5 used 4 cc masses
# # numCCM=5 # five cc masses were used for pz=(+/-)5
# cfgMin=1000
# cfgStep=4
# cfgMax=2956

# # # Some variables for the b6p1 24x64 lattices
# numCCM=3 # Runs with norm(momz)<5 used 3 cc masses
# # # numCCM=5 # five cc masses were used for pz=(+/-)5
# cfgMin=1000
# cfgStep=5
# cfgMax=1020

# Some variables for the b6p3 32x64 lattices
numCCM=3 # Runs with abs(momz) <= 1 use 3 cc masses
# numCCM=4 # Runs with 2 <= abs(momz) <= 4 use 4 cc masses
# numCCM=5 # Runs with abs(momz) >= 5 use 5 cc masses
cfgMin=1000
cfgStep=10
cfgMax=5160



momdir=""
if [ $momz -gt 0 ]
then
    momdir="pz+${momz}"
#elif [ $momz -lt 0 ]
#then
#    momdir="pz-${momz}"
else
    momdir="pz${momz}"
fi


# Make a suitable output directory
OUTDIR="/data/cpegerer/SDB_pion/${beta}/${ensemble}/${momdir}/${src}"
# OUTDIR="/data/cpegerer/SDB_pion/${beta}/${ensemble}/${momdir}/TRUTHRUNS_MELTEMI/${src}"
mkdir -p $OUTDIR

mkdir -p ${OUTDIR}/pion_2pt
mkdir -p ${OUTDIR}/h00z

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

$pyinf/write_modsdb_daemon_multikey.py -b $beta -e $ensemble -X $Nx -T $Nt -m $ml \
    -s $src -p $momz -q $momz -x $xi -u $xf -y $yi -v $yf -z $zi -w $zf -c $numCCM \
    -i $cfgMin -j $cfgMax -k $cfgStep > /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml


echo "Starting the extraction"
# Do all the extractions here
${HOME}/pionPDF/extraction/modsdb_daemon /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml \
    &>> /data/cpegerer/extraction/EXT_mom${momz}_${src}


exit 0
