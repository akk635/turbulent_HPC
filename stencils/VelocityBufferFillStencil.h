/*
 * VelocityBufferFillStencil.h
 *
 *  Created on: Dec 9, 2013
 *      Author: karthik
 */

#ifndef VELOCITYBUFFERFILLSTENCIL_H_
#define VELOCITYBUFFERFILLSTENCIL_H_
#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

class VelocityBufferFillStencil : public BoundaryStencil<FlowField> {

private:
	const static int *localSize;
public:

	// Initialize the buffers
	FLOAT* leftVelocityBuffer[3];
	FLOAT* rightVelocityBuffer[3];
	FLOAT* bottomVelocityBuffer[3];
	FLOAT* topVelocityBuffer[3];
	FLOAT* frontVelocityBuffer[3];
	FLOAT* backVelocityBuffer[3];
	struct xbuffer{
		FLOAT xVelocityBuffer[localSize[1] * localSize[2]];
		FLOAT yVelocityBuffer[(localSize[1]+1) * (localSize[2]+1)];
		FLOAT zVelocityBuffer[(localSize[1]+1) * (localSize[2]+1)];
	}leftXbuffer, rightXbuffer;

	VelocityBufferFillStencil( Parameters & parameters );
	~VelocityBufferFillStencil();

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


#endif /* VELOCITYBUFFERFILLSTENCIL_H_ */
