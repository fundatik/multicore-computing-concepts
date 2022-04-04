# How to Cite our Work

[[DOI]](https://doi.org/10.1109/PDP2018.2018.00120) D. Griebler, J. Loff, G. Mencagli, M. Danelutto and L. G. Fernandes. 

**Efficient NAS Benchmark Kernels with C++ Parallel Programming**. *In proceedings of the 26th Euromicro International 

Conference on Parallel, Distributed and Network-Based Processing (PDP)*. Cambridge, United Kingdom, 2018.

# The NPB-CPP Benchmark

These codes were converted to **C++** from the original [NPB3.3.1](https://doi.org/10.1109/PDP2018.2018.00120). We achieved similar performance in **C++** compared to the **Fortran** version.

This folder contains:

	- NPB-CLK - Directory with the parallel version implemented in Cilk
	- NPB-FF - Directory with the parallel version implemented in FastFlow
	- NPB-OMP - Directory with the parallel version translated from the original NPB version
	- NPB-SER - Directory with the serial version of the NPB ported to C++

Kernel:

	MG - Multi-Grid on a sequence of meshes, long- and short-distance communication, memory intensive

# Software Requiriments

*Warning: our tests were made with GCC-5*

**TBB**

*Installation*

	apt-get install libtbb-dev

**FastFlow** 

*Installation*

	svn co https://svn.code.sf.net/p/mc-fastflow/code/ $HOME/fastflow

# How to Compile 

Enter the directory from the version desired and execute:

	make mg CLASS=_VERSION														
_VERSIONs are: 	

	Class S: small for quick test purposes
	Class W: workstation size (a 90's workstation; now likely too small)	
	Classes A, B, C: standard test problems; ~4X size increase going from one class to the next	
	Classes D, E, F: large test problems; ~16X size increase from each of the previous Classes  

Command:

	make mg CLASS=B

###############################
# CSE 539 Final Project - Improving Scalability of the Cilk version of MG Benchmark  
# How to run experiments:

*On local machine:*

	svn co https://svn.code.sf.net/p/mc-fastflow/code/ $HOME/fastflow
	./build	
	./run_all

If it works on the local machine, than we can run it on the bsub machine. 

*On bsub machine:*

	./submit_job.sh
