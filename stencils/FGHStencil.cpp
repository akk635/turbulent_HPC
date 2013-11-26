#include "FGHStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

FGHStencil::FGHStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}


void FGHStencil::apply ( FlowField & flowField,  int i, int j ){

    // Load local velocities into the center layer of the local array

    loadLocalVelocity2D(flowField, _localVelocity, i, j);

    FLOAT* const values = flowField.getFGH().getVector(i,j);

    // Now the localVelocity array should contain lexicographically ordered elements around the
    // given index

    values [0] = computeF2D(_localVelocity, _parameters, _parameters.timestep.dt);
    values [1] = computeG2D(_localVelocity, _parameters, _parameters.timestep.dt);

}


void FGHStencil::apply ( FlowField & flowField, int i, int j, int k ){
    // The same as in 2D, with slight modifications

    const int obstacle = flowField.getFlags().getValue(i, j, k);

    FLOAT * const values = flowField.getFGH().getVector(i,j,k);

    if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

        loadLocalVelocity3D(flowField, _localVelocity, i, j, k);

        if ((obstacle & OBSTACLE_RIGHT) == 0) { // If the right cell is fluid
            values [0] = computeF3D(_localVelocity, _parameters, _parameters.timestep.dt);
        } else {
        	values [0] = flowField.getVelocity().getVector(i,j,k)[0];
        }
        if ((obstacle & OBSTACLE_TOP) == 0) {
            values [1] = computeG3D(_localVelocity, _parameters, _parameters.timestep.dt);
        } else {
        	values [1] = flowField.getVelocity().getVector(i,j,k)[1];
        }
        if ((obstacle & OBSTACLE_BACK) == 0) {
            values [2] = computeH3D(_localVelocity, _parameters, _parameters.timestep.dt);
        } else {
        	values [2] = flowField.getVelocity().getVector(i,j,k)[2];
        }
    } else {
    	if ((obstacle & OBSTACLE_RIGHT) != 0){
    		values [0] = flowField.getVelocity().getVector(i,j,k)[0];
    		flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i+1,j,k);
    	}
    	if ((obstacle & OBSTACLE_TOP) != 0) {
    		values [1] = flowField.getVelocity().getVector(i,j,k)[1];
    		flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j+1,k);
    	}
    	if ((obstacle & OBSTACLE_BACK) == 0) {
    		values [1] = flowField.getVelocity().getVector(i,j,k)[1];
    		flowField.getPressure().getScalar(i,j,k) = flowField.getPressure().getScalar(i,j,k+1);
    	}
    }
}
