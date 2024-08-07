all: various cvarious
	
various: various.s
	gcc -fuse-ld=bfd -nostartfiles -o various various.s

cvarious: cvarious.c various.s
	gcc -fuse-ld=bfd -c various.s -o various.o
	gcc -fuse-ld=bfd -o cvarious cvarious.c various.o
