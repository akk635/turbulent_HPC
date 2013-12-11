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
	// Initialize the buffers
	FLOAT* leftVelocityReadBuffer[3];
	FLOAT* rightVelocityReadBuffer[3];
	FLOAT* bottomVelocityReadBuffer[3];
	FLOAT* topVelocityReadBuffer[3];
	FLOAT* frontVelocityReadBuffer[3];
	FLOAT* backVelocityReadBuffer[3];

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
