source $(readlink -f setup.sh)

if [ ! -e build ]; then 
mkdir build
fi

cd build 
cmake ../
make install -j
source $(uname)/setup.sh

