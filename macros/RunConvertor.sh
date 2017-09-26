lndir=$LNDIR

#source root
source $lndir/fitter/lh-soft.sh
#set code directory
fitdir=$XSLLHFITTER/macros/
echo $fitdir

#set in/out file names
infile=$1
outfile=$2

if [[ $outfile =~ .root ]]; then
outfile=$(echo $outfile | rev | cut -c 6- | rev )
fi

if [ $3 ]; then
echo "RUNNING IN SCALE MODE"
scale=$3
outfile=$outfile\_wght$scale
fi

outfile=$outfile\_llFormat\.root

echo $outfile

#.root in file has to be in current directory
mydir=$(pwd)
dir=/Users/davidcoplowe/work/t2k/signal_extraction
infile=$dir/$infile

cd $dir
#make a directory for your output -- should be redundent now
if [ ! -d fitLLdata ]; then
    mkdir llFormat
fi

if [ $4 ]; then
    echo "in the pull directory: $4"
    if [ ! -d llFormat/$4 ]; then
        echo "making directory"
        mkdir llFormat/$4
    fi
    outfile=$dir/llFormat/$4/$outfile
else
    outfile=$dir/llFormat/$outfile
fi

#echo $infile
#echo $outfile

#treeConvert("$infile","default","$outfile", "dpTT", "truedpTT", "selmu_mom", "selmu_truemom")
#link code to current directory
ln -s $fitdir/treeConvert.* $dir
#ln -s $fitdir/Header.* $mydir

ls

if [ ! $3 ]; then
echo "Running"
root -l <<EOF
.L treeConvert.cc+
treeConvert t
t.Convert("$infile","default","$outfile", "dpTT", "truedpTT", "Enurec", "nu_trueE")
.q
EOF
echo "Complete"
else
root -l <<EOF
.L treeConvert.cc+
treeConvert t
t.Convert("$infile","default","$outfile", "dpTT", "truedpTT", "Enurec", "nu_trueE", true, $scale)
.q
EOF
fi

echo "Complete"

if [ $dir != $fitdir ]; then
rm treeConvert*
#rm Header*
fi

cd $mydir