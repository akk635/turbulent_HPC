#include "FGHStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

FGHStencil::FGHStencil(Parameters & parameters) :
		FieldStencil<FlowField>(parameters), BoundaryStencil<FlowField>(
				parameters), GlobalBoundaryFactory(parameters) {
}

void FGHStencil::apply(FlowField & flowField, int i, int j) {

	// Load local velocities into the center layer of the local array

	loadLocalVelocity2D(flowField, _localVelocity, i, j);

	FLOAT* const values = flowField.getFGH().getVector(i, j);

	// Now the localVelocity array should contain lexicographically ordered elements around the
	// given index

	values[0] = computeF2D(_localVelocity, FieldStencil<FlowField>::_parameters, FieldStencil<FlowField>::_parameters.timestep.dt);
	values[1] = computeG2D(_localVelocity, FieldStencil<FlowField>::_parameters, FieldStencil<FlowField>::_parameters.timestep.dt);

}

void FGHStencil::apply(FlowField & flowField, int i, int j, int k) {
	// The same as in 2D, with slight modifications

	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);
		loadLocalViscosity3D(flowField, _localViscosity, i, j, k);

		if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
			values[0] = computeF3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i + 1, j, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = computeG3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j + 1, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[2] = computeH3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[2] = flowField.getVelocity().getVector(i, j, k)[2];
			flowField.getPressure().getScalar(i, j, k + 1) =
					flowField.getPressure().getScalar(i, j, k);
		}
	} else {
		if ((obstacle & OBSTACLE_RIGHT) == 0) {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i + 1, j, k);
		}
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j + 1, k);
		}
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j, k + 1);
		}
	}
}

void FGHStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {

	if(FieldStencil<FlowField>::_parameters.parallel.leftNb < 0){
		_FGHStencils[0]->applyLeftWall ( flowField, i, j , k );
		return;
	}

	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);
        loadLocalViscosity3D(flowField, _localViscosity, i, j, k);

		if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
			values[0] = computeF3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i + 1, j, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
/*		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = computeG3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j + 1, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[2] = computeH3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[2] = flowField.getVelocity().getVector(i, j, k)[2];
			flowField.getPressure().getScalar(i, j, k + 1) =
					flowField.getPressure().getScalar(i, j, k);
		}*/
	} else {
		if ((obstacle & OBSTACLE_RIGHT) == 0) {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i + 1, j, k);
		}
/*		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j + 1, k);
		}
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j, k + 1);
		}*/
	}



}

void FGHStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k){
	if (FieldStencil<FlowField>::_parameters.parallel.rightNb < 0 ){
		_FGHStencils[1]->applyRightWall ( flowField, i, j , k );
		return;
	}
}

void FGHStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k ){

	if(FieldStencil<FlowField>::_parameters.parallel.bottomNb < 0){
		_FGHStencils[2]->applyBottomWall ( flowField, i, j , k );
		return;
	}

	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);
        loadLocalViscosity3D(flowField, _localViscosity, i, j, k);

/*		if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
			values[0] = computeF3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i + 1, j, k) =
					flowField.getPressure().getScalar(i, j, k);
		}*/
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = computeG3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j + 1, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
/*		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[2] = computeH3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[2] = flowField.getVelocity().getVector(i, j, k)[2];
			flowField.getPressure().getScalar(i, j, k + 1) =
					flowField.getPressure().getScalar(i, j, k);
		}*/
	} else {
/*		if ((obstacle & OBSTACLE_RIGHT) == 0) {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i + 1, j, k);
		}*/
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j + 1, k);
		}
/*		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j, k + 1);
		}*/
	}

}

void FGHStencil::applyTopWall ( FlowField & flowField, int i, int j, int k ){

	if(FieldStencil<FlowField>::_parameters.parallel.topNb < 0){
		_FGHStencils[3]->applyTopWall ( flowField, i, j , k );
	}
}

void FGHStencil::applyFrontWall  ( FlowField & flowField, int i, int j, int k ){

	if(FieldStencil<FlowField>::_parameters.parallel.frontNb < 0){
		_FGHStencils[4]->applyFrontWall ( flowField, i, j , k );
		return;
	}

	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);
        loadLocalViscosity3D(flowField, _localViscosity, i, j, k);

/*		if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
			values[0] = computeF3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i + 1, j, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = computeG3D(_localVelocity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j + 1, k) =
					flowField.getPressure().getScalar(i, j, k);
		}*/
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[2] = computeH3D(_localVelocity, _localViscosity, FieldStencil<FlowField>::_parameters,
					FieldStencil<FlowField>::_parameters.timestep.dt);
		} else {
			values[2] = flowField.getVelocity().getVector(i, j, k)[2];
			flowField.getPressure().getScalar(i, j, k + 1) =
					flowField.getPressure().getScalar(i, j, k);
		}
	} else {
/*		if ((obstacle & OBSTACLE_RIGHT) == 0) {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i + 1, j, k);
		}
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j + 1, k);
		}*/
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j, k + 1);
		}
	}

}

void FGHStencil::applyBackWall   ( FlowField & flowField, int i, int j, int k ){
	if(FieldStencil<FlowField>::_parameters.parallel.backNb < 0){
		_FGHStencils[5]->applyBackWall ( flowField, i, j , k );
		return;
	}
}


