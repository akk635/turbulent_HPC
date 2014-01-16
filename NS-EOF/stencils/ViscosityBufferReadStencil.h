/*
 * ViscosityBufferReadStencil.h
 *
 *  Created on: Jan 15, 2014
 *      Author: mahyar
 */


#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

class ViscosityBufferReadStencil : public BoundaryStencil<FlowField> {

private:

public:
	const int *localSize;
	// Initialize the buffers
	FLOAT* leftViscosityReadBuffer;
	FLOAT* rightViscosityReadBuffer;
	FLOAT* bottomViscosityReadBuffer;
	FLOAT* topViscosityReadBuffer;
	FLOAT* frontViscosityReadBuffer;
	FLOAT* backViscosityReadBuffer;

	ViscosityBufferReadStencil( Parameters & parameters );
	~ViscosityBufferReadStencil();

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
