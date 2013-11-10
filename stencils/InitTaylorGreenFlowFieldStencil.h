/*
 * InitTaylorGreenFlowFieldStencil.h
 *
 *  Created on: Nov 6, 2013
 *      Author: karthik
 */

#ifndef INITTAYLORGREENFLOWFIELDSTENCIL_H_
#define INITTAYLORGREENFLOWFIELDSTENCIL_H_

#include "../Parameters.h"
#include "../DataStructures.h"
#include "../FlowField.h"
#include "../Stencil.h"
#include <math.h>

class InitTaylorGreenFlowFieldStencil: public FieldStencil<FlowField> {
	private:
		FLOAT *velocity;
		int * Size;
	public:
		InitTaylorGreenFlowFieldStencil (const Parameters & parameters): FieldStencil<FlowField> ( parameters ){
			if(_parameters.geometry.dim == 2){
				velocity = NULL;
				Size = new int [2];
				Size[0] = _parameters.geometry.sizeX;
				Size[1] = _parameters.geometry.sizeY;
			}
			else if(_parameters.geometry.dim == 3) {
				velocity = NULL;
				Size = new int [3];
				Size[0] = _parameters.geometry.sizeX;
				Size[1] = _parameters.geometry.sizeY;
				Size[2] = _parameters.geometry.sizeZ;
			}
		}

		~InitTaylorGreenFlowFieldStencil(){
			velocity = NULL;
			delete[] Size;
		}

		// For initialising the 2d velocity fields
		void apply ( FlowField & flowField, int i, int j){

		}

		// ??For initialising the 3d velocity fields
		void apply ( FlowField & flowField, int i, int j, int k ) {
			//
			velocity = flowField.getVelocity().getVector(i, j, k);
			velocity[0] = cos (2 * M_PI * (i-1) / Size[0]) * sin ( 2 * M_PI * (j-1) / Size[1] ) * sin ( 2 * M_PI * (k-1) / Size[2] );
			velocity[1] = sin (2 * M_PI * (i-1) / Size[0]) * cos ( 2 * M_PI * (j-1) / Size[1] ) * sin ( 2 * M_PI * (k-1) / Size[2] );
			velocity[2] = sin (2 * M_PI * (i-1) / Size[0]) * sin ( 2 * M_PI * (j-1) / Size[1] ) * cos ( 2 * M_PI * (k-1) / Size[2] );
		}
};


#endif /* INITTAYLORGREENFLOWFIELDSTENCIL_H_ */
