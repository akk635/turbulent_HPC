/*
 * PressureBufferReadStencil.h
 *
 *  Created on: Dec 15, 2013
 *      Author: master
 */

#ifndef PRESSUREBYFFERREADSTENCIL_H_
#define PRESSUREBYFFERREADSTENCIL_H_


#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

class PressureBufferReadStencil : public BoundaryStencil<FlowField> {

private:

public:
	const int *localSize;
	// Initialize the buffers
	FLOAT* leftPressureReadBuffer;
	FLOAT* rightPressureReadBuffer;
	FLOAT* bottomPressureReadBuffer;
	FLOAT* topPressureReadBuffer;
	FLOAT* frontPressureReadBuffer;
	FLOAT* backPressureReadBuffer;

	PressureBufferReadStencil( Parameters & parameters );
	~PressureBufferReadStencil();

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



#endif /* PRESSUREBYFFERREADSTENCIL_H_ */
