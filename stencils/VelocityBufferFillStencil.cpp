/*
 * VelocityBufferFillStencil.cpp
 *
 *  Created on: Dec 9, 2013
 *      Author: karthik
 */

#include "VelocityBufferFillStencil.h"

VelocityBufferFillStencil::VelocityBufferFillStencil(Parameters & parameters) :
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {
	int dim = parameters.geometry.dim;
	leftVelocityBuffer = new FLOAT* [dim];
	rightVelocityBuffer = new FLOAT* [dim];
	bottomVelocityBuffer = new FLOAT* [dim];
	topVelocityBuffer = new FLOAT* [dim];
	frontVelocityBuffer = new FLOAT* [dim];
	backVelocityBuffer = new FLOAT* [dim];

	// Giving the max memory corresponding to the points
	leftVelocityBuffer[0] = new FLOAT[(localSize[1]) * (localSize[2])];
	rightVelocityBuffer[0] = new FLOAT[(localSize[1]) * (localSize[2])];
	for (int i = 1; i < dim; i++) {
		leftVelocityBuffer[i] = new FLOAT[(localSize[1] + 1)
				* (localSize[2] + 1)];
		rightVelocityBuffer[i] = new FLOAT[(localSize[1] + 1)
				* (localSize[2] + 1)];
	}

	bottomVelocityBuffer[1] = new FLOAT[(localSize[0]) * (localSize[2])];
	topVelocityBuffer[1] = new FLOAT[(localSize[0]) * (localSize[2])];
	for (int i = 0; i < dim; i += 2) {
		bottomVelocityBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[2] + 1)];
		topVelocityBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[2] + 1)];
	}

	frontVelocityBuffer[2] = new FLOAT[(localSize[0]) * (localSize[1])];
	backVelocityBuffer[2] = new FLOAT[(localSize[0]) * (localSize[1])];
	for (int i = 0; i < dim - 1; i++) {
		frontVelocityBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[1] + 1)];
		backVelocityBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[1] + 1)];
	}
}

void VelocityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	// Representing the outermost boundary for each of the
/*	if (i == 0) {
		leftVelocityBuffer[0][(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		// Has to be array with z as major direction
		leftVelocityBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
		leftVelocityBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[2];
	} else if (i == 1) {
		leftVelocityBuffer[1][k - 2] = (flowField.getVelocity().getVector(i, j,
				k))[1];
		leftVelocityBuffer[2][j - 2] = (flowField.getVelocity().getVector(i, j,
				k))[2];
	}*/
}
void VelocityBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
}
void VelocityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
}
void VelocityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
}
void VelocityBufferFillStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
}
void VelocityBufferFillStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
}

void applyLeftWall(FlowField & flowField, int i, int j) {
}
;
void applyRightWall(FlowField & flowField, int i, int j) {
}
;
void applyBottomWall(FlowField & flowField, int i, int j) {
}
;
void applyTopWall(FlowField & flowField, int i, int j) {
}
;

