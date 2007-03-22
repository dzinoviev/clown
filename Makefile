all:
	make -C uCVM
	make -C cas
	make -C clink
	make -C devices
	make -C clown
	make -C makedisc

clean:
	make -C uCVM clean
	make -C cas clean
	make -C clink clean
	make -C devices clean
	make -C clown clean
	make -C makedisc clean
