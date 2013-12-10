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
	const int *localSize;
public:
	// Initialize the buffers
	FLOAT** leftVelocityBuffer;
	FLOAT** rightVelocityBuffer;
	FLOAT** bottomVelocityBuffer;
	FLOAT** topVelocityBuffer;
	FLOAT** frontVelocityBuffer;
	FLOAT** backVelocityBuffer;


	VelocityBufferFillStencil( Parameters & parameters );
	~VelocityBufferFillStencil(){};

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
