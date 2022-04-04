SHELL=/bin/sh
CLASS=S
SFILE=config/suite.def

default: header
	@ $(SHELL) sys/print_instructions

common/libcilkrts.a:
	cp /project/linuxlab/class/cse539/cilkplus-rts/lib/libcilkrts.a ./common/libcilkrts.a 

MG: mg		       
mg: header common/libcilkrts.a	
	cd MG; $(MAKE) CLASS=$(CLASS)
		       
# Awk script courtesy cmg@cray.com
suite:
	@ awk '{ if ($$1 !~ /^#/ &&  NF > 0)                              \
	printf "make %s CLASS=%s\n", $$1, $$2 }' $(SFILE)  \
	| $(SHELL)


# It would be nice to make clean in each subdirectory (the targets
# are defined) but on a really clean system this will won't work
# because those makefiles need config/make.def
clean:
	- rm -f core 
	- rm -f *~ */core */*~ */*.o */npbparams.hpp */*.obj */*.exe
	- rm -f sys/setparams sys/makesuite sys/setparams.hpp

cleanall: clean
	- rm -r bin/*
	- rm -f common/libcilkrts.a

veryclean: clean
	- rm config/make.def config/suite.def Part*
	- rm bin/mg.*

header:
	@ $(SHELL) sys/print_header

kit: 
	- makekit -s100k -k30 * */* */*/*


