ifndef DESTDIR
DESTDIR = ..
endif

all:
	make -C codelib
	make -C devlib
	make -C cas
	make -C clink
	make -C clown
	make -C makedisc
	make -C clodd

clean:
	make -C codelib clean
	make -C devlib clean
	make -C cas clean
	make -C clink clean
	make -C clown clean
	make -C makedisc clean
	make -C clodd clean
	rm -rf bin
	rm -rf include/clown/config.h

install:
	make -C cas install DESTDIR=$(DESTDIR)
	make -C clink install DESTDIR=$(DESTDIR)
	make -C clown install DESTDIR=$(DESTDIR)
	make -C makedisc install DESTDIR=$(DESTDIR)
	make -C clodd install DESTDIR=$(DESTDIR)
	make -C man install DESTDIR=$(DESTDIR)

dist:
	(cd ..; mv clown clown-1.0.2; tar cvf clown-1.0.2.tar clown-1.0.2 --exclude '*.svn*' --exclude 'clob*'; gzip clown-1.0.2.tar; mv clown-1.0.2 clown)
