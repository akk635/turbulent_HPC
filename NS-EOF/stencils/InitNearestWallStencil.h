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

			FLOAT distance = i * _parameters.geometry.dx;
			if ( ( _parameters.geometry.sizeX - i ) * _parameters.geometry.dx < distance ){
				distance = ( _parameters.geometry.sizeX - i ) * _parameters.geometry.dx;
			}
			if ( j * _parameters.geometry.dy < distance ){
				distance = j * _parameters.geometry.dy;
			}
			if ( ( _parameters.geometry.sizeY - j ) * _parameters.geometry.dy < distance ){
				distance = ( _parameters.geometry.sizeY - j ) * _parameters.geometry.dy;
			}
			if ( k * _parameters.geometry.dz < distance ){
				distance = k * _parameters.geometry.dz;
			}
			if ( ( _parameters.geometry.sizeZ - k ) * _parameters.geometry.dz < distance ){
				distance = ( _parameters.geometry.sizeZ - k ) * _parameters.geometry.dz;
			}

			flowField.getDistanceToNearWall().getScalar (i, j, k) = distance;
		}
};


#endif /* INITNEARESTWALLSTENCIL_H_ */
