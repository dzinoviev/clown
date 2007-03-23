CLOWN
Microcomputer Simulator for Operating System Research


Description:

The CLOWN virtual machine is a microcomputer simulator for operating research, designed and implemented by Professor D. Zinoviev at the Math and Computer Science Department at Suffolk University.

CLOWN attempts to emulate an essentially realistic, though vastly simplified computer system.

CLOWN is still a work in progress, now once again under active development after a lengthy hiatus. The code base is stable, although not yet feature complete.


Compatibility:

CLOWN is known to compile and run on several variants of Linux, as well as Mac OS X. It should work in any reasonably sane *nix environment, and if not please let us know. A Windows port is planned, and should be underway soon.

The Gnu readline library is also required.

Building:

We are assuming for now that you are running some form of *nix (including OS X).

Make sure that the readline library is accessible to the linker (modify LD_LIBRARY_PATH if necessary).
Type the following at the terminal prompt:
	
	cd <src-root>
	make all

Where <src-root> is the path to the directory containing the source distribution. Binaries will be built in the <src-root>/bin/ directory.

Notes:

Set the environment variable CLOWN to <src-root> before running the bin/cas assembler

License:

This distribution of CLOWN is placed under the MIT license (see license.txt in this same directory), though a free use license may be available from Dmitry.

Copyright and contact:

Developed by Professor Dmitry Zinoviev (Suffolk University)
dzinoviev@gmail.com

Further work by Tristam MacDonald
swiftcoder@gmail.com
