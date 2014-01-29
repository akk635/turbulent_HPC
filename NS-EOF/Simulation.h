#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <petscksp.h>
#include <float.h>
#include "FlowField.h"
#include <assert.h>

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
//WORKSHEET 5
#include "stencils/InitNearestWallStencil.h"
#include "stencils/InitBoundLayerThicknessStencil.h"
#include "stencils/MaxViscosityStencil.h"
#include "stencils/TurbulentViscosityStencil.h"

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
    // Just for the internal boundaries
    ParallelBoundaryIterator<FlowField> fghparallelbndItr;

    RHSStencil RHS;
    FieldIterator<FlowField> globalRHSFieldIterator;

    VelocityStencil newvelocities;
    FieldIterator<FlowField> NewVelocitiesUpdateIterator;
    GlobalBoundaryIterator<FlowField> velocityboundaryIterator;
    ParallelBoundaryIterator<FlowField> velocityparallelbndItr;

    // TODO WORKSHEET 2: add instance of PetscSolver here
    PetscSolver petscsolver;

    // TODO WORKSHEET 3: add instance of PetscParallelManager
    MessagePassingConfiguration comm;
    int rank;

    //WORKSHEET %
    MaxViscosityStencil MaxViscosity;

    FieldIterator<FlowField> MaxViscosityFlowFieldIterator;

    TurbulentViscosityStencil turbulentViscosity;
    FieldIterator<FlowField> globalturbulentViscosityFieldIterator;

 public:
    Simulation( Parameters &parameters, FlowField &flowField )
            : _parameters( parameters ),
              _flowField( flowField ),
              MaxU( parameters ),
              MaxUFlowFieldIterator( _flowField, _parameters, MaxU, 1, 0 ),
              MaxUBoundaryIterator( _flowField, _parameters, MaxU, 1, 0 ),

              FGH( _parameters ),
              globalFGHFieldIterator( _flowField, _parameters, FGH, 1, 0 ),
              fghboundaryIterator( _flowField, _parameters, FGH, 1, 0 ),
              fghparallelbndItr( _flowField, _parameters, FGH, 1, -1 ),

              RHS( _parameters ),
              globalRHSFieldIterator( _flowField, _parameters, RHS, 1, 0 ),

              petscsolver( _flowField, _parameters ),

              newvelocities( _parameters ),
              NewVelocitiesUpdateIterator( _flowField, _parameters, newvelocities, 1, 0 ),
              comm( _parameters, _flowField ),
              velocityboundaryIterator( _flowField, _parameters, newvelocities, 1, 0 ),
              velocityparallelbndItr( _flowField, _parameters, newvelocities, 1, 0 ),

              MaxViscosity( parameters ),
              MaxViscosityFlowFieldIterator( _flowField, _parameters, MaxViscosity, 1, 0 ),
              turbulentViscosity( _parameters ),
              globalturbulentViscosityFieldIterator( _flowField, _parameters, turbulentViscosity, 1,
                                                     0 )

    {
        MPI_Comm_rank( PETSC_COMM_WORLD, &rank );
    }

    virtual ~Simulation() {
    }

    virtual void solveTimestep() {
        // TODO WORKSHEET 2: reset maximum velocity values and determine new maximum values

        MaxU.reset();
        MaxUFlowFieldIterator.iterate();
        // For covering the domain boundary
        MaxUBoundaryIterator.iterate();

        //WORKSHEET 5
        if ( _parameters.turbulent.turbulent_scenario != "laminar" ) {
            globalturbulentViscosityFieldIterator.iterate();
            comm.communicateViscosity();
            MaxViscosity.reset();
            MaxViscosityFlowFieldIterator.iterate();
        }
        //END OF WORKSHEET 5 section

        // TODO WORKSHEET 2: set new time step
        setTimeStep();

        // TODO WORKSHEET 2: compute fgh
        globalFGHFieldIterator.iterate();
        fghparallelbndItr.iterate();
        fghboundaryIterator.iterate();

        // TODO WORKSHEET 2: compute the right hand side
        // Iterated only in the internal domain
        globalRHSFieldIterator.iterate();

        // TODO WORKSHEET 2: solve for pressure poisson equation
        petscsolver.solve();

        // TODO WORKSHEET 3: communicate pressure values after solving the pressure Poisson equation
        comm.communicatePressure();

        // TODO WORKSHEET 2: compute velocity update (time stepping)
        NewVelocitiesUpdateIterator.iterate();

        velocityparallelbndItr.iterate();
        // TODO WORKSHEET 2: update velocity values on the boundary
        velocityboundaryIterator.iterate();

        // TODO WORKSHEET 3: communicate velocity values after velocity update is finished
        comm.communicateVelocity();
        // For checking the boundary velocity
        //velocityboundaryIterator.testItrX();
    }

    void initializeVelocity() {
        InitTaylorGreenFlowFieldStencil taylorGreenStencil( _parameters );
        FieldIterator<FlowField> InitTaylorGreenFlowFieldIterator( _flowField, _parameters,
                                                                   taylorGreenStencil, 0, 0 );
        InitTaylorGreenFlowFieldIterator.iterate();
    }

    void initFlagField() {
        BFStepInitStencil bfFlagFieldStencil( _parameters );
        FieldIterator<FlowField> bfFlagFieldIterator( _flowField, _parameters, bfFlagFieldStencil,
                                                      0, 0 );
        bfFlagFieldIterator.iterate();

    }

    void initializeNearestWall() {
        InitNearestWallStencil nearestWallStencil( _parameters );
        FieldIterator<FlowField> InitNearestWallIterator( _flowField, _parameters,
                                                          nearestWallStencil, 1, 0 );
        InitNearestWallIterator.iterate();
    }

    void initializeBoundLayerThickness() {
        InitBoundLayerThicknessStencil boundLayerThicknessStencil( _parameters );
        FieldIterator<FlowField> InitBoundLayerThicknessIterator( _flowField, _parameters,
                                                                  boundLayerThicknessStencil, 0,
                                                                  0 );
        InitBoundLayerThicknessIterator.iterate();
    }

    /** plots the flow field.  */
    void plotVTK( int timeStep, int rank ) {
        // TODO WORKSHEET 1
        VTKStencil _vtk( _parameters );
        FieldIterator<FlowField> VtkIterator( _flowField, _parameters, _vtk, 1, 0 );
        _vtk.write( timeStep, rank );
        VtkIterator.iterate();
        _vtk.writeFinished();
    }

 protected:
    /** sets the time step according to the maximum velocity values that have been determined before */
    void setTimeStep() {

        // TODO WORKSHEET 2: determine maximum time step according to CFL-condition and maximum velocity values;
        //                   set the respective timestep in _parameters.timestep.dt.

        if ( _parameters.timestep.tau > 0 ) {
            FLOAT a = 1, b = 1, c = 1, d = 1;
            a =
                    ( 1 / ( MaxViscosity.getMaxValues() + 1.0 / _parameters.flow.Re ) )
                            / ( 2.0
                                    * ( 1.0 / ( _parameters.geometry.dx * _parameters.geometry.dx )
                                            + 1.0
                                                    / ( _parameters.geometry.dy
                                                            * _parameters.geometry.dy )
                                            + 1.0
                                                    / ( _parameters.geometry.dz
                                                            * _parameters.geometry.dz ) ) );

            if ( ( MaxU.getMaxValues() )[0] > 0 ) {
                b = _parameters.geometry.dx / ( MaxU.getMaxValues() )[0];
            }
            if ( ( MaxU.getMaxValues() )[1] > 0 ) {
                c = _parameters.geometry.dy / ( MaxU.getMaxValues() )[1];
            }
            if ( ( MaxU.getMaxValues() )[2] > 0 ) {
                d = _parameters.geometry.dz / ( MaxU.getMaxValues() )[2];
            }

            if ( _parameters.timestep.dt > a ) {
                _parameters.timestep.dt = _parameters.timestep.tau * a;
            }
            if ( _parameters.timestep.dt > b ) {
                _parameters.timestep.dt = _parameters.timestep.tau * b;
            }
            if ( _parameters.timestep.dt > c ) {
                _parameters.timestep.dt = _parameters.timestep.tau * c;
            }
            if ( _parameters.timestep.dt > d ) {
                _parameters.timestep.dt = _parameters.timestep.tau * d;
            }
        }

        // TODO WORKSHEET 3: determine global minimum of time step

        //check that output are exactly at the time they are printing
        /*        if (_parameters.simulation.currentTime + _parameters.timestep.dt > (_parameters.vtk.vtkCounter + 1) * _parameters.vtk.interval-0.001){
         _parameters.timestep.dt = (_parameters.vtk.vtkCounter + 1) * _parameters.vtk.interval - _parameters.simulation.currentTime;

         }*/

        // MPI_Allreduce (&sendbuf,&recvbuf,count,datatype,op,comm)
        MPI_Allreduce( MPI_IN_PLACE, &( _parameters.timestep.dt ), 1, MY_MPI_FLOAT, MPI_MIN,
                       PETSC_COMM_WORLD );

    }

};

#endif // _SIMULATION_H_

