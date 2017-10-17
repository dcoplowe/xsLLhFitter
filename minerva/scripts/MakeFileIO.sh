# source $(readlink -f ~/.profile)
# SHOULD BE THIS DIR:
mydir=$(pwd)
cd ${mydir}

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

if [ -z "${rootfile}" ]; then
	echo "No root file defined"
	input_error
	return
fi

if [ -z "${treename}" ]; then
	echo "No tree defined"
	input_error
	return
fi

# This could be improved (using PyRoot):
root -l -b <<EOF
TFile file("${rootfile}")
TTree * intree = (TTree*)file.Get("${treename}")
intree->MakeClass();
EOF

# cppfile="FileIO.cpp"
cat > FileIO.cpp <<EOF
$(cat FileIO_Maker.cpp)

void FileIO::Init()
{
	// Set branch addresses and branch pointers
    fChain->SetMakeClass(1);
    
$(grep "fChain->SetBranchAddress" ${treename}.h)

    // This is so that we always have the size elements initialised.
    fChain->GetEntry(0);
}
#endif
EOF

# Make Header file:CCProtonPi0(TTree *tree=0);
find_first="Declaration of leaf types"
find_last="${treename}(TTree"
# echo "find_last = ${find_last}"
# Get block of variables and their respective branches:
first_line=$(grep -n "${find_first}" ${treename}.h | awk '{print $1}')
first_line=${first_line//:}
# echo "first_line = ${first_line}"
last_line=$(grep -n "${find_last}" ${treename}.h  | head -n 1 | awk '{print $1}')
last_line=${last_line//:}
last_line=$(expr ${last_line} - 1 )
# echo "last_line = ${last_line}"
# goodlines=$(sed -n ${first_line},${last_line}p ${treename}.h)

cat > FileIO.h <<EOF
$(cat FileIO_Maker.h)

	$(sed -n ${first_line},${last_line}p ${treename}.h)

};
#endif
EOF

rm ${treename}.{C,h}

if [ ! -d ${mydir}/old ]; then
	mkdir -p ${mydir}/old
fi

# Copy over FileIO.{cpp,h} to their correct locations:
for ii in cpp h; do
	if cmp --silent FileIO.${ii} ${mydir}/../src/fileio/FileIO.${ii}
	then
		echo "FileIO.{ii} is unchanged, will not copy to fileio directory."
		cp ${mydir}/../src/fileio/FileIO.${ii} ${mydir}/old/FileIO.${ii}_old$(date "+%d%m%g")
		cp FileIO.${ii} ${mydir}/../src/fileio/FileIO.${ii}
	fi
	rm FileIO.${ii}
done