#!/bin/bash
#PBS -N ext
#PBS -l nodes=2:ppn=24
#PBS -l walltime=180:00:00


ml=-0.2800
momz=ZZZ
src=SSS
xi=-4
xf=4
yi=-4
yf=4
zi=-5
zf=5

ensemble=EEE
beta="b6p1"
Nx=24
Nt=64

exttype="hxyz"
# exttype="hx0z"


# # Some variables for the b6p1 32x96 lattices                                                                   
# numCCM=4 # Runs with norm(momz)<5 used 4 cc masses                                                             
# # numCCM=5 # five cc masses were used for pz=(+/-)5                                                            
# cfgMin=1000                                                                                                    
# cfgStep=4                                                                                                      
# cfgMax=2956                                                                                                    

# Some variables for the b6p1 24x64 lattices                                                                   
numCCM=3 # Runs with norm(momz)<5 used 3 cc masses                                                             
# numCCM=5 # five cc masses were used for pz=(+/-)5                                                            
cfgMin=1000                                                                                                    
cfgStep=5                                                                                                      
# cfgMax=1005
cfgMax=11615

# # Some variables for the b6p3 32x64 lattices                                                                   
# # numCCM=3 # Runs with abs(momz) <= 1 use 3 cc masses                                                         
# numCCM=4 # Runs with 2 <= abs(momz) <= 4 use 4 cc masses                                                       
# # numCCM=5 # Runs with abs(momz) >= 5 use 5 cc masses                                                          
# cfgMin=1000
# cfgStep=10
# cfgMax=5160



momdir=""
if [ $momz -gt 0 ]
then
    momdir="pz+${momz}"
else
    momdir="pz${momz}"
fi


# Make a suitable output directory
OUTDIR="/data/cpegerer/SDB_pion/${beta}/${ensemble}/${momdir}/${src}"
mkdir -p $OUTDIR

mkdir -p ${OUTDIR}/pion_2pt
mkdir -p ${OUTDIR}/${exttype}


export OMP_NUM_THREADS=24


cd $PBS_O_WORKDIR
echo "Working in $PBS_O_WORKDIR"

# Source the environment
source ${HOME}/qcd_software/cyclops/scalar/env.sh

# Use a python interface to generate the xml
pyinf=${HOME}/structure/special_keys

$pyinf/write_modsdb_daemon_TMDkey.py -b $beta -e $ensemble -m $ml -X $Nx -T $Nt \
    -s $src -p $momz -q $momz -x $xi -u $xf -y $yi -v $yf -z $zi \
    -w $zf -c $numCCM -i $cfgMin -j $cfgMax \
    -k $cfgStep -a $exttype > /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml


echo "Starting the extraction"
# Do all the extractions here
${HOME}/pionPDF/extraction/modsdb_daemon /data/cpegerer/extraction/runxmls/daemon_${PBS_JOBID}.ini.xml \
    &>> /data/cpegerer/extraction/EXT_mom${momz}_${src}_${exttype}_tmd


exit 0
