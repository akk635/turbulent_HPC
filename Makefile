# comment the following two lines for mac-cluster
#include ${PETSC_DIR}/${PETSC_ARCH}/conf/variables
#include ${PETSC_DIR}/${PETSC_ARCH}/conf/rules
# uncomment the following two lines for mac-cluster
#include ${PETSC_DIR}/conf/variables
#include ${PETSC_DIR}/conf/rules

# default gnu compiler (currently not used)
# CC = g++
# compiler wrapper for mac-cluster
#CC = mpiCC
# compiler on Ubuntu
CC = mpic++
CFLAGS = -Wall  -O3
SRCDIR = ../src
BUILDDIR = build
PETSC_DIR = /usr/local/petsc
# uncomment the following two lines for mac-cluster
include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules

INCLUDE = -I. -Istencils -I${PETSC_DIR}/${PETSC_ARCH}/include -I${PETSC_DIR}/include

NSMAIN = main.o

OBJ = Configuration.o 3rdparty/tinyxml2/tinyxml2.o SimpleTimer.o

NSOBJ = FlowField.o LinearSolver.o stencils/MaxUStencil.o stencils/MovingWallStencils.o stencils/PeriodicBoundaryStencils.o\
stencils/FGHStencil.o solvers/SORSolver.o solvers/PetscSolver.o \
stencils/RHSStencil.o stencils/VelocityStencil.o \
GlobalBoundaryFactory.o \
parallelManagers/PetscParallelConfiguration.o \
stencils/BFStepInitStencil.o stencils/NeumannBoundaryStencils.o stencils/BFInputStencils.o \
visual.o

all: ns

ns: $(OBJ) $(NSOBJ) $(NSMAIN)
	$(CC) -o ns $(OBJ) $(NSOBJ) $(NSMAIN) $(PETSC_KSP_LIB) -lstdc++ $(CFLAGS)


%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $*.o $*.cpp $(PETSC_KSP_LIB) -lstdc++

# Name clean made unavailable by Petsc
cleanall:
	for name in  ns main.o $(NSOBJ) testbin $(OBJ) tests/*.o; do \
	if [ -f $$name ]; then rm $$name; fi; \
	done;
