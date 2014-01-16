/*
 * ViscosityBufferFillStencil.h
 *
 *  Created on: Jan 15, 2014
 *      Author: mahyar
 */


#include <fstream>
#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <mpi.h>
#include <sstream>

class ViscosityBufferFillStencil : public BoundaryStencil<FlowField> {

private:

public:
	const int *localSize;
	// Initialize the buffers
	FLOAT* leftViscosityBuffer;
	FLOAT* rightViscosityBuffer;
	FLOAT* bottomViscosityBuffer;
	FLOAT* topViscosityBuffer;
	FLOAT* frontViscosityBuffer;
	FLOAT* backViscosityBuffer;

	std::ofstream fpi;
	ViscosityBufferFillStencil( Parameters & parameters );
	~ViscosityBufferFillStencil();

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

    void applyTest ( FlowField & flowField, int i, int j, int k );

};

