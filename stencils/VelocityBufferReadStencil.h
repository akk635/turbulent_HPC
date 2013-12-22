/*
 * VelocityBufferReadStencil.h
 *
 *  Created on: Dec 11, 2013
 *      Author: karthik
 */

#ifndef VELOCITYBUFFERREADSTENCIL_H_
#define VELOCITYBUFFERREADSTENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

class VelocityBufferReadStencil : public BoundaryStencil<FlowField> {

private:
	const int *localSize;
public:
	// fIRST FILLING UP X, Y and Z
	// Initialize the buffers
	FLOAT* leftVelocityReadBuffer;
	FLOAT* rightVelocityReadBuffer;
	FLOAT* bottomVelocityReadBuffer;
	FLOAT* topVelocityReadBuffer;
	FLOAT* frontVelocityReadBuffer;
	FLOAT* backVelocityReadBuffer;

	VelocityBufferReadStencil( Parameters & parameters );
	~VelocityBufferReadStencil();

	void applyLeftWall   ( FlowField & flowField, int i, int j );
	void applyRightWall  ( FlowField & flowField, int i, int j );
	void applyBottomWall ( FlowField & flowField, int i, int j );
	void applyTopWall    ( FlowField & flowField, int i, int j );

    void applyLeftWall   ( FlowField & flowField, int i, int j, int k );
    void applyRightWall  ( FlowField & flowField, int i, int j, int k );
    void applyBottomWall ( FlowField & flowField, int i, int j, int k );
    void applyTopWall    ( FlowField & flowField, int i, int j, int k );
    void applyFrontWall  ( FlowField & flowField, int i, int j, int k );
    void applyBackWall   ( FlowField & flowField, int i, int j, int k );

};

#endif /* VELOCITYBUFFERREADSTENCIL_H_ */
