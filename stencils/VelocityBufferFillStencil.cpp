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
		topVelocityBuffer[i] =
				new FLOAT[(localSize[0] + 1) * (localSize[2] + 1)];
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

VelocityBufferFillStencil::~VelocityBufferFillStencil() {
}

void VelocityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	// First internal layer of the sub-domain
	// Iterate with lowoffset = 1, highoffset = -1 while i = 2 is the subdomian boundary so (i+1) in the vector
	if ((j >= 2) & (k >= 2)) {
		leftVelocityBuffer[0][(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[0];
		leftVelocityBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
		leftVelocityBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[2];
	} else if (j == 1) {
		leftVelocityBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
	} else if (k == 1) {
		leftVelocityBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[2];
	}

	// Useful for the write Stencil
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
	// 	With lowoffset = 1 and highoffset = -1 since i = cellsX - 1 + highoffset
	if ((j >= 2) & (k >= 2)) {
		rightVelocityBuffer[0][(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i - 1, j, k))[0];
		rightVelocityBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		rightVelocityBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	} else if ((j == 1) & (k >= 2)) {
		rightVelocityBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
	} else if ((k == 1) & (j >= 2)) {
		rightVelocityBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	}
}
void VelocityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1 and highoffset = -1 with (j+1) in the vector access
	if ((i >= 2) & (k >= 2)) {
		bottomVelocityBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j+1, k))[0];
		bottomVelocityBuffer[1][(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j+1, k))[1];
		bottomVelocityBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j+1, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		bottomVelocityBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j+1, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		bottomVelocityBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j+1, k))[2];
	}

}
void VelocityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		topVelocityBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		topVelocityBuffer[1][(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j - 1, k))[1];
		topVelocityBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		topVelocityBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		topVelocityBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	}
}
void VelocityBufferFillStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1, highoffset = -1 and (k+1) vector access
	if ((i >= 2) & (j >= 2)) {
		frontVelocityBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k+1))[0];
		frontVelocityBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k+1))[1];
		frontVelocityBuffer[2][(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k+1))[2];
	} else if ((i == 1) & (j >= 2)) {
		frontVelocityBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k+1))[0];
	} else if ((j == 1) & (i >= 2)) {
		frontVelocityBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k+1))[1];
	}
}
void VelocityBufferFillStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	// Remains the same with the lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		backVelocityBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		backVelocityBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		backVelocityBuffer[2][(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k - 1))[2];
	} else if ((i == 1) & (j >= 2)) {
		backVelocityBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((j == 1) & (i >= 2)) {
		backVelocityBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
	}
}

void VelocityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}

