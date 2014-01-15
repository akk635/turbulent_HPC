# comment the following two lines for mac-cluster
#include ${PETSC_DIR}/${PETSC_ARCH}/conf/variables
#include ${PETSC_DIR}/${PETSC_ARCH}/conf/rules
# uncomment the following two lines for mac-cluster
# PETSC_DIR = /usr/local/petsc
include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules

# default gnu compiler (currently not used)
# CC = g++
# compiler wrapper for mac-cluster
#CC = mpiCC
# compiler on Ubuntu
CC = mpic++
CFLAGS = -Wall  -O3
SRCDIR = ../src
BUILDDIR = build

INCLUDE = -I. -Istencils -I${PETSC_DIR}/${PETSC_ARCH}/include -I${PETSC_DIR}/include

NSMAIN = main.o

OBJ = Configuration.o 3rdparty/tinyxml2/tinyxml2.o SimpleTimer.o

NSOBJ = FlowField.o LinearSolver.o stencils/MaxUStencil.o stencils/MovingWallStencils.o stencils/PeriodicBoundaryStencils.o\
stencils/FGHStencil.o solvers/SORSolver.o solvers/PetscSolver.o \
stencils/RHSStencil.o stencils/VelocityStencil.o \
GlobalBoundaryFactory.o  stencils/testStencil.o \
parallelManagers/PetscParallelConfiguration.o \
stencils/BFStepInitStencil.o stencils/NeumannBoundaryStencils.o stencils/BFInputStencils.o \
visual.o stencils/VelocityBufferFillStencil.o  parallelManagers/MessagePassingConfiguration.o \
stencils/VelocityBufferReadStencil.o stencils/PressureBufferReadStencil.o stencils/PressureBufferFillStencil.o \

all: ns

ns:  $(OBJ) $(NSOBJ) $(NSMAIN) 
	$(CC) -o ns  $(OBJ) $(NSOBJ) $(NSMAIN) $(PETSC_KSP_LIB)  -lstdc++ $(CFLAGS)


%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $*.o $*.cpp $(PETSC_KSP_LIB) -lstdc++

# Name clean made unavailable by Petsc
cleanall:
	for name in  ns main.o $(NSOBJ) testbin $(OBJ) tests/*.o; do \
	if [ -f $$name ]; then rm $$name; fi; \
	done;\
	rm *.vtk;\
	rm *.dat;
