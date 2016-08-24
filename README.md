# TACS README #

The Toolkit for the Analysis of Composite Structures is a parallel finite-element code for analysis and gradient-based design of advanced structures. Originally, it was primarily designed as a tool for the analysis of shell structures, such as wing-boxes. More recently it has been extended to perform topology optimization of large three-dimensional structures using gradient-based methods. 

# Setting up and installing TACS #

In addition to a working implementation of MPI, BLAS and LAPACK, TACS has two external dependencies that must be installed before TACS can be compiled:

1. Metis 4.1: For parallel mesh partitioning
2. AMD/UFconfig: For approximate minimum degree (AMD) matrix reordering
3. numpy: numerical python package
4. mpi4py: python package for interfacing with MPI

Metis and AMD/UFconfig are installed under tacs/extern. There is a README file in this directory with instructions on how to setup and install these packages correctly. In addition, to convert TACS FH5 output files to tecplot-compatible files, you must (unfortunately) install TecIO. This can be placed in the tacs/extern directory. 

Once the external dependencies are installed, copy Makefile.in.info to Makefile.in. Open Makefile.in and follow the directions within to set the variables. In particular, set the following:

1. TACS_DIR: the root directory of TACS
2. CXX: the C++ compiler - must be MPI-enabled
3. LAPACK_LIBS: the linking arguments for the LAPACK libraries
4. METIS_INCLUDE/METIS_LIB: the include/linking arguments for METIS
5. AMD_INCLUDE/AMD_LIBS: the include/linking arguments for AMD
6. PYTHON_INCLUDE the python include path
7. NUMPY_DIR: the numpy include directory 
8. MPI4PY_DIR: the directory for mpi4py header files

Note that the default values can often be used without modification. Of all these options, it is most important for performance reasons to link to an optimized version of LAPACK, if possible.

### Converting FH5 files ###

The utility f5totec can be used to convert the .f5 files generated by TACS to tecplot .plt files. This utility is located under tacs/extern/f5totec/. I find it convenient to create a symbolic link to f5totec in a bin directory.

### General information ###

TACS is written and maintained by Graeme Kennedy: graeme.kennedy@ae.gatech.edu

If you use TACS, please cite the paper:

Kennedy, G. J. and Martins, J. R. R. A, "A parallel finite-element framework for large-scale gradient-based design optimization of high-performance structures", Finite Elements in Analysis and Design, 2014, doi:http://dx.doi.org/10.1016/j.finel.2014.04.011