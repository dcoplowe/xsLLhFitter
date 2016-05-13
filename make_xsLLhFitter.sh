
if [ $(uname) == Darwin ]; then
	SETUP=setup.sh
elif [ $(uname) == Linux ]; then
 	SETUP=$(readlink -f setup.sh)
else 
	SETUP=$(readlink -f setup.sh)
fi

source $SETUP

if [ ! -e build ]; then 
mkdir build
fi

cd build 
cmake ../
make install -j
source $(uname)/setup.sh

