/*
 * PressureBufferFillStencil.h
 *
 *  Created on: Dec 15, 2013
 *      Author: master
 */

#ifndef PRESSUREBUFFERFILLSTENCIL_H_
#define PRESSUREBUFFERFILLSTENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

class PressureBufferFillStencil : public BoundaryStencil<FlowField> {

private:

public:
	const int *localSize;
	// Initialize the buffers
	FLOAT* leftPressureBuffer;
	FLOAT* rightPressureBuffer;
	FLOAT* bottomPressureBuffer;
	FLOAT* topPressureBuffer;
	FLOAT* frontPressureBuffer;
	FLOAT* backPressureBuffer;

	PressureBufferFillStencil( Parameters & parameters );
	~PressureBufferFillStencil();

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






#endif /* PRESSUREBUFFERFILLSTENCIL_H_ */
