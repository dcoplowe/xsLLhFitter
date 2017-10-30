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

cp ${mydir}/../src/fileio/FileIO.{h,cpp} ${mydir}
ls ${mydir}
#---------------------------------------- FileIO .cpp file ----------------------------------------

# In cpp file we want to find and replace only the Init part
find_first="FileIO::Init()"
first_line=$(grep -n "${find_first}" FileIO.cpp  | head -n 1 | awk '{print $1}')
first_line=${first_line%%:*}
# Get block of variables and their respective branches:
length=$(sed -n -e '/Init/,$p' FileIO.cpp | grep -n "}")
length=${length%%:*}
last_line=$(expr ${first_line} + ${length} - 1)

echo "first_line = ${first_line}"
echo "last_line = ${last_line}"
sed "$first_line,$last_line d" FileIO.cpp > FileIO.cpp_tmp 

# cppfile="FileIO.cpp"
cat > FileIO.cpp <<EOF
$(cat FileIO.cpp_tmp | grep -v \#endif) 

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

#---------------------------------------- FileIO .h file ----------------------------------------

# Now for the header file:
find_first="__DO_NOT_DELETE_COMMENTED_LINE__"
first_line=$(grep -n "${find_first}" FileIO.h  | head -n 1 | awk '{print $1}')
first_line=${first_line%%:*}
if [ "$first_line" -eq "$first_line" ] 2>/dev/null; then
  	echo "Found number $first_line"
else
  	echo "No line found: $find_first"
  	find_first="Declaration of leaf types"
  	echo "Checking for $find_first"
	first_line=$(grep -n "${find_first}" FileIO.h  | head -n 1 | awk '{print $1}')
	first_line=${first_line%%:*}
	if [ "$first_line" -eq "$first_line" ] 2>/dev/null; then
  		echo "Found number $first_line"
  	fi
fi

sed "$first_line,$ d" FileIO.h > FileIO.h_tmp

# Make Header file:CCProtonPi0(TTree *tree=0);
find_first="Declaration of leaf types"
find_last="${treename}(TTree"
# echo "find_last = ${find_last}"
# Get block of variables and their respective branches:
first_line=$(grep -n "${find_first}" ${treename}.h | awk '{print $1}')
first_line=${first_line%%:*}
# echo "first_line = ${first_line}"
last_line=$(grep -n "${find_last}" ${treename}.h  | head -n 1 | awk '{print $1}')
last_line=${last_line%%:*}
last_line=$(expr ${last_line} - 1 )

cat > FileIO.h <<EOF
$(cat FileIO.h_tmp)
	
   	$(echo // ${find_first})

	$(sed -n ${first_line},${last_line}p ${treename}.h)

};
#endif
EOF

# Find and replace all arrays and set with larger size:

sed -i -e 's/\[.*\]\;/\[__MAX_ARRAY_SIZE__\]\;/g' FileIO.h

rm ${treename}.{C,h}

if [ ! -d ${mydir}/old ]; then
	mkdir -p ${mydir}/old
fi

# Copy over FileIO.{cpp,h} to their correct locations:
for ii in cpp h; do
	echo "Checking file: FileIO.${ii}"
	if cmp --silent FileIO.${ii} ${mydir}/../src/fileio/FileIO.${ii}
	then
		echo "FileIO.${ii} is unchanged, will not copy to fileio directory."
	else
		cp ${mydir}/../src/fileio/FileIO.${ii} ${mydir}/old/FileIO.${ii}_old$(date "+%d%m%g")
		cp FileIO.${ii} ${mydir}/../src/fileio/FileIO.${ii}
	fi
	rm FileIO.${ii} FileIO.${ii}_tmp
done