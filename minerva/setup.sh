source $(readlink -f ~/.profile)
anasoft

export DATA_DIR=/minerva/data/users/$USER/xsLLFitter_IO

if [ ! -d ${DATA_DIR} ]; then
	mkdir -p ${DATA_DIR}
fi