#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <stdio.h>
#include <petscksp.h>
#include <float.h>
#include "FlowField.h"
// TODO WORKSHEET 1: include VTKStencil here
// TODO WORKSHEET 2: include all stencils, iterators etc. here
#include "Iterators.h"
#include "Definitions.h"
#include "stencils/InitTaylorGreenFlowFieldStencil.h"
#include "stencils/VTKStencil.h"
#include "MaxUStencil.h"
#include "GlobalBoundaryFactory.h"
// TODO WORKSHEET 3: include PetscParallelManager



class Simulation {
protected:
	Parameters &_parameters;

	FlowField &_flowField;

	// TODO WORKSHEET 2: add instances of stencils and iterators here

	// TODO WORKSHEET 2: add instance of PetscSolver here

	// TODO WORKSHEET 3: add instance of PetscParallelManager



public:
	Simulation(Parameters &parameters, FlowField &flowField):
		_parameters(parameters),
		_flowField(flowField)
	// TODO WORKSHEET 2: initialize stencils, iterators and pressure solver here

	// TODO WORKSHEET 3: initialize instance of PetscParallelManager
	{
		// TODO WORKSHEET 2: set up flag field for backward facing step scenario (if required)
	}

	virtual ~Simulation(){}

	virtual void solveTimestep(){
		// TODO WORKSHEET 2: reset maximum velocity values and determine new maximum values
		MaxUStencil MaxU = MaxUStencil(_parameters);
		MaxU.reset();
		FieldIterator<FlowField> MaxUFlowFieldIterator( _flowField, _parameters, MaxU,0, 0);
		MaxUFlowFieldIterator.iterate();
		MaxU.getMaxValues();


		// TODO WORKSHEET 2: set new time step
		setTimeStep(MaxU.getMaxValues());
		//std::cout <<"this is the time step:"<< _parameters.timestep.dt<<std::endl;

		// TODO WORKSHEET 2: compute fgh

		// TODO WORKSHEET 2: set global boundary values for fgh

		// TODO WORKSHEET 2: compute the right hand side

		// TODO WORKSHEET 2: solve for pressure poisson equation

		// TODO WORKSHEET 3: communicate pressure values after solving the pressure Poisson equation

		// TODO WORKSHEET 2: compute velocity update (time stepping)

		// TODO WORKSHEET 3: communicate velocity values after velocity update is finished

		// TODO WORKSHEET 2: update velocity values on the boundary
	}

	/** plots the flow field.  */
	void plotVTK(int timeStep){
		// TODO WORKSHEET 1
		// Setting up periodic boundary conditions
		GlobalBoundaryFactory globalBoundary = GlobalBoundaryFactory(_parameters);
		GlobalBoundaryIterator<FlowField> taylorGreenBoundaryIterator = globalBoundary.getGlobalBoundaryVelocityIterator( _flowField );
		taylorGreenBoundaryIterator.iterate();

		InitTaylorGreenFlowFieldStencil taylorGreenStencil(_parameters);
		FieldIterator<FlowField> InitTaylorGreenFlowFieldIterator( _flowField, _parameters, taylorGreenStencil,
				0, 0);
		InitTaylorGreenFlowFieldIterator.iterate();

		VTKStencil _vtk(_parameters);
		_vtk.write ( _flowField, timeStep );
		FieldIterator<FlowField> VtkIterator(_flowField, _parameters, _vtk, 0, 0);
		VtkIterator.iterate();
	}

protected:
	/** sets the time step according to the maximum velocity values that have been determined before */
	void setTimeStep(const FLOAT * maxvelocities){

		// TODO WORKSHEET 2: determine maximum time step according to CFL-condition and maximum velocity values;
		//                   set the respective timestep in _parameters.timestep.dt.

		/*Determines the minimum of dt according to stability criteria and multiply it by safety factor tau if tau is positive, otherwise uses the default value of dt*/
		if (_parameters.timestep.tau>0){
			double a,b,c,d;
			a = _parameters.flow.Re/(2.0*(1.0/(_parameters.geometry.dx*_parameters.geometry.dx)
					+1.0/(_parameters.geometry.dy*_parameters.geometry.dy)+1.0/(_parameters.geometry.dz*_parameters.geometry.dz)));
			b = _parameters.geometry.dx/maxvelocities[0];
			c = _parameters.geometry.dy/maxvelocities[1];
			d = _parameters.geometry.dz/maxvelocities[2];
			if(a < b && a < c && a<d){
				_parameters.timestep.dt = _parameters.timestep.tau * a;
			}
			else if(b < a && b < c && b<d){
				_parameters.timestep.dt = _parameters.timestep.tau * b;
			}
			else if(c < a && c < b && b<d){
				_parameters.timestep.dt = _parameters.timestep.tau * c;
			}
			else{
				_parameters.timestep.dt = _parameters.timestep.tau * d;
			}
		}
	}

	// TODO WORKSHEET 3: determine global minimum of time step


};

#endif // _SIMULATION_H_

