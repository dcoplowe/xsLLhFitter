#!/bin/sh Test commit

#We will set up to build in a subdir of the source tree
#If it was sourced as . setup.sh then you can't scrub off the end... assume that
#we are in the correct directory.
RLINK="pwd"
export XSLLINPUTDIR=/minerva/data/users/$USER/xsLLFitter_IO

if [ ! -d ${XSLLINPUTDIR} ]; then
	mkdir -p ${XSLLINPUTDIR}
fi

#Need to setup the relevent baths for build:
minerva_version=v10r8p12
#May need to check the root version exists in the min version -- could write something to so this automatically
root_version="5.34.05"
#DEBUG="-dbg"
DEBUG="-opt"

intstallroot=/grid/fermiapp/minerva/software_releases/${minerva_version}
export LGCEXTERNAL=${intstallroot}/lcg/external

#LD path:
export LD_LIBRARY_PATH=/usr/lib64:${LD_LIBRARY_PATH}
#Setup minerva specific cmake
CMAKEROOT=${LGCEXTERNAL}/cmake_sl6/bin
export PATH=${PATH}:${CMAKEROOT}

#Set up root:i
export CMTCONFIG="x86_64-slc6-gcc44${DEBUG}"
LocalRootDir=${LGCEXTERNAL}/ROOT/${root_version}/${CMTCONFIG}/bin
echo $LocalRootDir
source $(readlink -f ${LocalRootDir}/thisroot.sh)

which root

if ! echo "${BASH_SOURCE}" | grep --silent "/"; then
    SETUPDIR=$($RLINK)
else
  SETUPDIR=$(readlink -f ${BASH_SOURCE%/*})
fi

export XSLLHFITTER=${SETUPDIR}

BUILDSETUP="${XSLLHFITTER}/build/$(uname)/setup.sh"

echo "[INFO]: XSLLHFITTER root expected at: ${XSLLHFITTER}"

source ${XSLLHFITTER}/cmake/CMakeSetup.sh

cd $SETUPDIR

if [ ! -e ${BUILDSETUP} ]; then
  echo "[INFO]: Cannot find build setup script where expected: ${BUILDSETUP}"
else
  echo "[INFO]: Sourcing build setup script."
  source ${BUILDSETUP}
  echo "[INFO]: \$ which CCQEFit: $(which CCQEFit)"
fi

unset SETUPDIR
unset BUILDSETUP
