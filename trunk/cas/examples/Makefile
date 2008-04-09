all: offset

offset: clowns.s jump.s
	cas -e 4096 clowns.s
	cas jump.s
	@echo
	@echo "Running CLOWN belong this line:"
	@echo "-------------------------------"
	@clown -q -r jump.cle -b clowns.cle 4096
	@rm jump.cle clowns.cle
