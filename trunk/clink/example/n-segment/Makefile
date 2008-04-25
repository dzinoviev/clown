AS = cas
LD = clink
RUN = @clown

all: test clean

%.clo: %.s
	$(AS) -s $*

example.cle: segment-isrA.clo  segment-isrB.clo
	$(LD) segment-isrA.clo  segment-isrB.clo -o example.cle

test: example.cle
	@echo
	@echo "Running CLOWN belong this line:"
	@echo "-------------------------------"
	$(RUN) -r -q example.cle

clean:
	@rm -f *.cle *.clo *.dbg *~
