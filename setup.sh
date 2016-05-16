#!/bin/sh Test commit

#We will set up to build in a subdir of the source tree
#If it was sourced as . setup.sh then you can't scrub off the end... assume that
#we are in the correct directory.

if [ $(uname) == Darwin ]; then
    RLINK="pwd"
	source ~/software/ROOT/v5r34p34n00/Darwin/bin/thisroot.sh
elif [ $(uname) == Linux ]; then
    RLINK="readlink -f $(pwd)"
    module load root
else
    RLINK="readlink -f $(pwd)"
fi

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
