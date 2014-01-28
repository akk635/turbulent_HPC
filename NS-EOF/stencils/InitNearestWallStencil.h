/*
 * InitNearestWallStencil.h
 *
 *  Created on: Jan 14, 2014
 *      Author: Valizadeh
 */

#ifndef INITNEARESTWALLSTENCIL_H_
#define INITNEARESTWALLSTENCIL_H_

#include "../Parameters.h"
#include "../DataStructures.h"
#include "../FlowField.h"
#include "../Stencil.h"
#include <math.h>

class InitNearestWallStencil: public FieldStencil<FlowField> {

public:
	InitNearestWallStencil (const Parameters & parameters): FieldStencil<FlowField> ( parameters ){}

	//		~InitNearestWallStencil(){
	//		}

	// For initialising the 2d case
	void apply ( FlowField & flowField, int i, int j){

	}

	// For initialising the 3d case
	void apply ( FlowField & flowField, int i, int j, int k ) {

		FLOAT distance = ( _parameters.geometry.sizeY - (_parameters.parallel.firstCorner[1] + (j-2+0.5)) ) * _parameters.geometry.dy;
		FLOAT temp;
		if ( (temp=(_parameters.parallel.firstCorner[1] + (j-2+0.5)) * _parameters.geometry.dy) < distance ){
			distance = temp;
		}
		if ( (temp=(_parameters.parallel.firstCorner[2] + (k-2+0.5)) * _parameters.geometry.dz) < distance ){
			distance = temp;
		}
		if ( (temp=( _parameters.geometry.sizeZ - (_parameters.parallel.firstCorner[2] + (k-2+0.5)) ) * _parameters.geometry.dz) < distance ){
			distance = temp;
		}

		if (_parameters.simulation.scenario != "channel"){
			if ( (temp=( _parameters.geometry.sizeX - (_parameters.parallel.firstCorner[0] + (i-2+0.5)) ) * _parameters.geometry.dx) < distance ){
				distance = temp;
			}
			if ( (temp=(_parameters.parallel.firstCorner[0] + (i-2+0.5)) * _parameters.geometry.dx) < distance ){
				distance = temp;
			}
		}

		flowField.getDistanceToNearWall().getScalar (i, j, k) = distance;
	}
};


#endif /* INITNEARESTWALLSTENCIL_H_ */
