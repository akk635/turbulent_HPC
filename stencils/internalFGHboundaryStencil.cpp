/*
 * internalFGHboundaryStencil.cpp

 *
 *  Created on: Dec 22, 2013
 *      Author: karthik
 */

#include "internalFGHboundaryStencil.h"

internalFGHboundaryStencil::internalFGHboundaryStencil(Parameters & parameters) :
		BoundaryStencil<FlowField>(parameters), GlobalBoundaryFactory(
				parameters) {

	std::cout<<"To check" << parameters.walls.typeLeft <<std::endl;

}

void internalFGHboundaryStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);

		if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
			values[0] = computeF3D(_localVelocity, _parameters,
					_parameters.timestep.dt);
		} else {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i + 1, j, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		/*        if ((obstacle & OBSTACLE_TOP) == 0) {
		 values [1] = computeG3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j+1,k) = flowField.getPressure().getScalar(i,j,k);
		 }
		 if ((obstacle & OBSTACLE_BACK) == 0) {
		 values [2] = computeH3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [2] = flowField.getVelocity().getVector(i,j,k)[2];
		 flowField.getPressure().getScalar(i,j,k+1) = flowField.getPressure().getScalar(i,j,k);
		 }*/
	} else {
		if ((obstacle & OBSTACLE_RIGHT) == 0) {
			values[0] = flowField.getVelocity().getVector(i, j, k)[0];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i + 1, j, k);
		}
		/*    	if ((obstacle & OBSTACLE_TOP) == 0) {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j+1,k);
		 }
		 if ((obstacle & OBSTACLE_BACK) == 0) {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j,k+1);
		 }*/
	}
}

void internalFGHboundaryStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {

}

void internalFGHboundaryStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);

		/*
		 if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
		 values [0] = computeF3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [0] = flowField.getVelocity().getVector(i,j,k)[0];
		 flowField.getPressure().getScalar(i+1,j,k) = flowField.getPressure().getScalar(i,j,k);
		 }
		 */
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = computeG3D(_localVelocity, _parameters,
					_parameters.timestep.dt);
		} else {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j + 1, k) =
					flowField.getPressure().getScalar(i, j, k);
		}
		/*        if ((obstacle & OBSTACLE_BACK) == 0) {
		 values [2] = computeH3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [2] = flowField.getVelocity().getVector(i,j,k)[2];
		 flowField.getPressure().getScalar(i,j,k+1) = flowField.getPressure().getScalar(i,j,k);
		 }*/
	} else {
		/*    	if ((obstacle & OBSTACLE_RIGHT) == 0){
		 values [0] = flowField.getVelocity().getVector(i,j,k)[0];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i+1,j,k);
		 }*/
		if ((obstacle & OBSTACLE_TOP) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j + 1, k);
		}
		/*    	if ((obstacle & OBSTACLE_BACK) == 0) {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j,k+1);
		 }*/
	}
}

void internalFGHboundaryStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {

	const int obstacle = flowField.getFlags().getValue(i, j, k);

	FLOAT * const values = flowField.getFGH().getVector(i, j, k);

	if ((obstacle & OBSTACLE_SELF) == 0) {   // If the cell is fluid

		loadLocalVelocity3D(flowField, _localVelocity, i, j, k);

		/*
		 if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
		 values [0] = computeF3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [0] = flowField.getVelocity().getVector(i,j,k)[0];
		 flowField.getPressure().getScalar(i+1,j,k) = flowField.getPressure().getScalar(i,j,k);
		 }

		 if ((obstacle & OBSTACLE_TOP) == 0) {
		 values [1] = computeG3D(_localVelocity, _parameters, _parameters.timestep.dt);
		 } else {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j+1,k) = flowField.getPressure().getScalar(i,j,k);
		 }*/
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[2] = computeH3D(_localVelocity, _parameters,
					_parameters.timestep.dt);
		} else {
			values[2] = flowField.getVelocity().getVector(i, j, k)[2];
			flowField.getPressure().getScalar(i, j, k + 1) =
					flowField.getPressure().getScalar(i, j, k);
		}
	} else {
		/*    	if ((obstacle & OBSTACLE_RIGHT) == 0){
		 values [0] = flowField.getVelocity().getVector(i,j,k)[0];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i+1,j,k);
		 }
		 if ((obstacle & OBSTACLE_TOP) == 0) {
		 values [1] = flowField.getVelocity().getVector(i,j,k)[1];
		 flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j+1,k);
		 }*/
		if ((obstacle & OBSTACLE_BACK) == 0) {
			values[1] = flowField.getVelocity().getVector(i, j, k)[1];
			flowField.getPressure().getScalar(i, j, k) =
					flowField.getPressure().getScalar(i, j, k + 1);
		}
	}
}

