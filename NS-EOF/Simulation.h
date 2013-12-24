#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <petscksp.h>
#include <float.h>
#include "FlowField.h"
#include <time.h>
#include <unistd.h>

// TODO WORKSHEET 1: include VTKStencil here
// TODO WORKSHEET 2: include all stencils, iterators etc. here
#include "Iterators.h"
#include "Definitions.h"
#include "stencils/InitTaylorGreenFlowFieldStencil.h"
#include "stencils/VTKStencil.h"
#include "stencils/MaxUStencil.h"
#include "GlobalBoundaryFactory.h"
#include "stencils/MaxUStencil.h"
#include "stencils/FGHStencil.h"
#include "stencils/RHSStencil.h"
#include "stencils/VelocityStencil.h"
#include "stencils/BFStepInitStencil.h"
#include "stencils/VelocityBufferFillStencil.h"
#include "solvers/PetscSolver.h"

// TODO WORKSHEET 3: include PetscParallelManager
#include "parallelManagers/MessagePassingConfiguration.h"

class Simulation {
 protected:
    Parameters &_parameters;

    FlowField &_flowField;

    // TODO WORKSHEET 2: add instances of stencils and iterators here
    MaxUStencil MaxU;
    // with the low_offset = 1 only for the internal cells
    FieldIterator<FlowField> MaxUFlowFieldIterator;
    GlobalBoundaryIterator<FlowField> MaxUBoundaryIterator;

    FGHStencil FGH;
    // Named global valid only in the computation over entire domain
    FieldIterator<FlowField> globalFGHFieldIterator;
    GlobalBoundaryIterator<FlowField> fghboundaryIterator;

    GlobalBoundaryFactory globalBoundary;

    RHSStencil RHS;
    FieldIterator<FlowField> globalRHSFieldIterator;

    VelocityStencil newvelocities;
    FieldIterator<FlowField> NewVelocitiesUpdateIterator;
    GlobalBoundaryIterator<FlowField> velocityboundaryIterator;

    // TODO WORKSHEET 2: add instance of PetscSolver here
    PetscSolver petscsolver;

    // TODO WORKSHEET 3: add instance of PetscParallelManager
    MessagePassingConfiguration comm;

 public:
    Simulation(Parameters &parameters, FlowField &flowField):
        _parameters(parameters),
        _flowField(flowField),
        MaxU(parameters),
        MaxUFlowFieldIterator(_flowField, parameters, MaxU, 1, 0),
        MaxUBoundaryIterator(_flowField, parameters, MaxU, 1, 0),
        FGH( _parameters ),
        globalFGHFieldIterator( _flowField, parameters, FGH, 1, 0),

        globalBoundary( _parameters ),

        RHS( _parameters ),
        globalRHSFieldIterator( _flowField, parameters, RHS, 1, 0),
        petscsolver( _flowField, _parameters ),
        newvelocities( _parameters ),
        NewVelocitiesUpdateIterator( _flowField, _parameters, newvelocities,1 , 0),
        comm( _parameters, _flowField),
        fghboundaryIterator(_flowField, parameters, FGH, 1, 0),
        velocityboundaryIterator(_flowField, parameters, newvelocities, 1, 0)

 // TODO WORKSHEET 2: initialize stencils, iterators and pressure solver here
 // TODO WORKSHEET 3: initialize instance of PetscParallelManager
 {
        // TODO WORKSHEET 2: set up flag field for backward facing step scenario (if required)
 }

    virtual ~Simulation(){}

    virtual void solveTimestep(){
        // TODO WORKSHEET 2: reset maximum velocity values and determine new maximum values


        MaxU.reset();
        MaxUFlowFieldIterator.iterate();
        // For covering the domain boundary
        MaxUBoundaryIterator.iterate();


        // TODO WORKSHEET 2: set new time step
        setTimeStep();

        // TODO WORKSHEET 2: compute fgh
        globalFGHFieldIterator.iterate();

        // velocityfillIterator.iterate();

        fghboundaryIterator.iterate();

        int i = 22-1;
        std::cout<<"be zero FGH "<<_flowField.getVelocity().getVector(i, 10, 10)[0]<<std::endl;

        // TODO WORKSHEET 2: set global boundary values for fgh
        // globalBoundary.getGlobalBoundaryFGHIterator(_flowField).iterate();
        // For the internal boundaries and the external boundaries

        std::cout<<"afetr bdny"<<std::endl;

        // TODO WORKSHEET 2: compute the right hand side
        // Iterated only in the internal domain
        globalRHSFieldIterator.iterate();

        std::cout<<"b4 solev"<<std::endl;

        // TODO WORKSHEET 2: solve for pressure poisson equation
        petscsolver.solve();

        std::cout<<"afetr  solve"<<std::endl;

        // TODO WORKSHEET 3: communicate pressure values after solving the pressure Poisson equation
        comm.communicatePressure();

        // TODO WORKSHEET 2: compute velocity update (time stepping)
        NewVelocitiesUpdateIterator.iterate();
        // TODO WORKSHEET 3: communicate velocity values after velocity update is finished
        comm.communicateVelocity();

        // TODO WORKSHEET 2: update velocity values on the boundary
        // globalBoundary.getGlobalBoundaryVelocityIterator(_flowField).iterate();
        velocityboundaryIterator.iterate();
        i = 22-1;
        std::cout<<"be zero "<<_flowField.getVelocity().getVector(i, 10, 10)[0]<<std::endl;
    }

    void initializeVelocity(){
        InitTaylorGreenFlowFieldStencil taylorGreenStencil( _parameters );
        FieldIterator<FlowField> InitTaylorGreenFlowFieldIterator( _flowField, _parameters, taylorGreenStencil,
                                                                   0, 0 );
        InitTaylorGreenFlowFieldIterator.iterate();
    }

    void initFlagField(){
        BFStepInitStencil bfFlagFieldStencil( _parameters );
        FieldIterator<FlowField> bfFlagFieldIterator(_flowField, _parameters, bfFlagFieldStencil, 0, 0);
        bfFlagFieldIterator.iterate();

    }

    /** plots the flow field.  */
    void plotVTK(int timeStep, int rank){
        // TODO WORKSHEET 1
        VTKStencil _vtk(_parameters);
        FieldIterator<FlowField> VtkIterator(_flowField, _parameters, _vtk, 1, 0);
        _vtk.write ( _flowField, timeStep, rank );
        VtkIterator.iterate();
        _vtk.writeFinished();
    }

 protected:
    /** sets the time step according to the maximum velocity values that have been determined before */
    void setTimeStep(){

        int rank;   // This processor's identifier
        int nproc;  // Number of processors in the group
        MPI_Comm_size(PETSC_COMM_WORLD, &nproc);
        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

        // TODO WORKSHEET 2: determine maximum time step according to CFL-condition and maximum velocity values;
        //                   set the respective timestep in _parameters.timestep.dt.
        if (_parameters.timestep.tau>0){
            FLOAT a=1,b=1,c=1,d=1;
            a = _parameters.flow.Re/(2.0*(1.0/(_parameters.geometry.dx*_parameters.geometry.dx)
                    +1.0/(_parameters.geometry.dy*_parameters.geometry.dy)+1.0/(_parameters.geometry.dz*_parameters.geometry.dz)));
            if ( ( MaxU.getMaxValues() )[0]>0 && ( MaxU.getMaxValues() )[1]>0 && ( MaxU.getMaxValues() )[2]>0){
            b = _parameters.geometry.dx/( MaxU.getMaxValues() )[0];
            c = _parameters.geometry.dy/( MaxU.getMaxValues() )[1];
            d = _parameters.geometry.dz/( MaxU.getMaxValues() )[2];
            }

            _parameters.timestep.dt = 1;
            if ( _parameters.timestep.dt > a ){
            	_parameters.timestep.dt = _parameters.timestep.tau * a;
                std::cout << "rank :" << rank << "dt_a = " << _parameters.timestep.dt << std::endl;

            }
            if (_parameters.timestep.dt > b ){
            	_parameters.timestep.dt = _parameters.timestep.tau * b;
                std::cout << "rank :" << rank << "dt_b = " << _parameters.timestep.dt << std::endl;

            }
            if (_parameters.timestep.dt > c ){
            	_parameters.timestep.dt = _parameters.timestep.tau * c;
                std::cout << "rank :" << rank << "dt_c = " << _parameters.timestep.dt << std::endl;

            }
            if (_parameters.timestep.dt > d){
            	_parameters.timestep.dt = _parameters.timestep.tau * d;
                std::cout << "rank :" << rank << "dt_d = " << _parameters.timestep.dt << std::endl;

            }
        }

        // TODO WORKSHEET 3: determine global minimum of time step
        // MPI_Allreduce (&sendbuf,&recvbuf,count,datatype,op,comm)
        MPI_Allreduce( MPI_IN_PLACE, &(_parameters.timestep.dt), 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

        //check that output are exactly at the time they are printing
/*        if (_parameters.simulation.currentTime + _parameters.timestep.dt > (_parameters.vtk.vtkCounter + 1) * _parameters.vtk.interval-0.001){
            _parameters.timestep.dt = (_parameters.vtk.vtkCounter + 1) * _parameters.vtk.interval - _parameters.simulation.currentTime;

        }*/
    }

};

#endif // _SIMULATION_H_


