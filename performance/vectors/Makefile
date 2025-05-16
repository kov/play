all:
	make -C veccpp -j`nproc`
	veccpp/build/veccpp
	cd vecrust; cargo bench
