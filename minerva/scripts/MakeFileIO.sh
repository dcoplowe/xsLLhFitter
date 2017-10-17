source $(readlink -f ~/.profile)

version="v10r8p12"
minbase="/grid/fermiapp/minerva/software_releases/${version}/setup.sh"

if [ -z ${ROOTSYS+x} ]; then 
	echo "No root version. Sourcing from minerva version ${version}"
	source ${minbase}
fi


input_error()
{
    echo "Run script as follow: ./MakeFileIO.sh root_file.root tree_name"
}

rootfile=$1
treename=$2

if [ -z ${rootfile+x} ]; then
	echo "No root file defined"
	input_error
	return
fi

if [ -z ${treename+x} ]; then
	echo "No tree defined"
	input_error
	return
fi

# This could be improved (using PyRoot):
root -l -b ${rootfile} <<EOF
TTree * intree = (TTree*)_file0->Get(${treename})
intree->MakeClass();
EOF