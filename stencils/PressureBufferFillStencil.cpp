/*
 * PressureBufferFillStencil.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: master
 */


#include "PressureBufferFillStencil.h"

PressureBufferFillStencil::PressureBufferFillStencil(Parameters &parameters):
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {

	int dim = parameters.geometry.dim;

	// Giving the max memory corresponding to the points
	leftPressureBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
	rightPressureBuffer = new FLOAT[(localSize[1]) * (localSize[2])];


	bottomPressureBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
	topPressureBuffer = new FLOAT[(localSize[0]) * (localSize[2])];


	frontPressureBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
	backPressureBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}

PressureBufferFillStencil::~PressureBufferFillStencil(){};

void PressureBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	// First internal layer of the sub-domain
	// Iterate with lowoffset = 1, highoffset = -1 while i = 2 is the subdomian boundary so (i+1) in the vector
	if ((j >= 2) & (k >= 2)) {
		leftPressureBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getPressure().getScalar(i + 1, j, k));
	}
}

void PressureBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	// 	With lowoffset = 1 and highoffset = -1 since i = cellsX - 1 + highoffset
	if ((j >= 2) & (k >= 2)) {
		rightPressureBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getPressure().getScalar(i - 1, j, k));
	}
}

void PressureBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1 and highoffset = -1 with (j+1) in the vector access
	if ((i >= 2) & (k >= 2)) {
		bottomPressureBuffer[(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getPressure().getScalar(i, j+1, k));
	}
}

void PressureBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		topPressureBuffer[(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getPressure().getScalar(i, j-1, k));
	}
}

void PressureBufferFillStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1, highoffset = -1 and (k+1) vector access
	if ((i >= 2) & (j >= 2)) {
		frontPressureBuffer[(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getPressure().getScalar(i, j, k+1));
	}
}
void PressureBufferFillStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	// Remains the same with the lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		backPressureBuffer[(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getPressure().getScalar(i, j, k-1));
	}
}

void PressureBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}


