AS = cas
LD = clink
RUN = @clown

all: test clean

stdio.clo: stdio.s
	$(AS) -s stdio.s

getsputs.clo: getsputs.s
	$(AS) -s getsputs.s

example.clo: getsputs.clo stdio.clo
	$(LD) -s getsputs.clo stdio.clo -o example.clo

example.cle: example.clo
	$(LD) example.clo -o example.cle

test: example.cle
	@echo
	$(RUN) -q -r example.cle

clean:
	@rm -f *.cle *.clo *.dbg *~
