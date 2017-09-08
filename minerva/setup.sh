source $(readlink -f ~/profile)
anasoft

export data_dir=/minerva/data/users/$USER/xsLLFitter_IO

if [ ! -d ${data_dir} ]; then
	mkdir -p ${data_dir}
fi