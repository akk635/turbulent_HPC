/*
 * maxVelocityStencil.h
 *
 *  Created on: Nov 15, 2013
 *      Author: karthik
 */

#ifndef MAXVELOCITYSTENCIL_H_
#define MAXVELOCITYSTENCIL_H_

#include "../Parameters.h"
#include "../DataStructures.h"
#include "../FlowField.h"
#include "../Stencil.h"
#include <math.h>

class maxVelocityStencil:public FieldStencil<FlowField> {
private:
	FLOAT maxVelocity;
public:
	maxVelocityStencil (const Parameters & parameters): FieldStencil<FlowField> ( parameters ){
		maxVelocity = 0;
	}
	~maxVelocityStencil(){

	}

	// For initialising the 3d velocity fields
	void apply ( FlowField & flowField, int i, int j, int k ) {
		//
		if ( maxVelocity < flowField.getVelocity().getVector(i, j, k)[ 0 ] ){
			maxVelocity = flowField.getVelocity().getVector(i, j, k)[ 0 ];
		}
		if ( )
	}

};




#endif /* MAXVELOCITYSTENCIL_H_ */
