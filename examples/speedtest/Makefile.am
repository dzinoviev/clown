## Process this file with automake to produce Makefile.in

nobase_dist_doc_DATA = speed.asm speed.s Makefile

clean-local:
	rm -f *.cle *.o speed

all:
	cas speed.s
	nasm -f elf64 speed.asm -o speed.o
	$(CC) -o speed speed.o	
	time clown -r -q speed.cle 
	time ./speed
