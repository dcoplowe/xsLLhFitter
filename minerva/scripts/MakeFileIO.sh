# source $(readlink -f ~/.profile)

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

replacewith_cpp=$(grep "fChain->SetBranchAddress" ${treename}.h)
# for ii in h cpp; do 
# 	cp FileIO_Maker.${ii} FileIO.${ii}
# done
cppfile="FileIO.cpp"
cat > ${cppfile} <<EOF
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



# # For init: Find every instance of fChain->SetBranchAddress(...) and put this in Init()
# # replace_h="__ADD_PUBLIC_VARS_AND_BRANCHES__"
# replace_cpp="__ADD_PUBLIC_VARS_AND_BRANCH_INITIALISATION_HERE__"

# # replacewith_h=$(grep "fChain->SetBranchAddress" ${treename}.h)
# 
# n_replacewith_cpp=$(${replacewith_cpp} | wc -l)
# echo "Adding  lines to FileIO.cpp"

# sed 's/${replace_cpp}/${replacewith_cpp}/g' FileIO.cpp
# # sed "s/${replace_cpp}/${replacewith_cpp}/g" FileIO.cpp
# # first=// Declaration of leaf types

# void FileIO::Init()
# {
# 	// Set branch addresses and branch pointers
#     fChain->SetMakeClass(1);

#     __ADD_PUBLIC_VARS_AND_BRANCH_INITIALISATION_HERE__
    
#     // This is so that we always have the size elements initialised.
#     fChain->GetEntry(0);
# }