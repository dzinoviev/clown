AS = cas
LD = clink
RUN = @clown

all: test clean

%.clo: %.s
	$(AS) -s $*

example.clo: main.clo stdio.clo
	$(LD) -s main.clo stdio.clo -o example.clo

example.cle: example.clo crt0.clo
	$(LD) crt0.clo example.clo -o example.cle

test: example.cle
	@echo
	@echo "Running CLOWN belong this line:"
	@echo "-------------------------------"
	$(RUN) -q -r example.cle

clean:
	@rm -f *.cle *.clo *.dbg *~
