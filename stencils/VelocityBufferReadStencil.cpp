/*
 * VelocityBufferReadStencil.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: karthik
 */
#include "VelocityBufferReadStencil.h"

VelocityBufferReadStencil::VelocityBufferReadStencil(Parameters & parameters) :
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {

	int dim = parameters.geometry.dim;

	// Giving the max memory corresponding to the points
	leftVelocityReadBuffer[0] = new FLOAT[(localSize[1]) * (localSize[2])];
	rightVelocityReadBuffer[0] = new FLOAT[(localSize[1]) * (localSize[2])];
	for (int i = 1; i < dim; i++) {
		leftVelocityReadBuffer[i] = new FLOAT[(localSize[1] + 1)
				* (localSize[2] + 1)];
		rightVelocityReadBuffer[i] = new FLOAT[(localSize[1] + 1)
				* (localSize[2] + 1)];
	}

	bottomVelocityReadBuffer[1] = new FLOAT[(localSize[0]) * (localSize[2])];
	topVelocityReadBuffer[1] = new FLOAT[(localSize[0]) * (localSize[2])];
	for (int i = 0; i < dim; i += 2) {
		bottomVelocityReadBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[2] + 1)];
		topVelocityReadBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[2] + 1)];
	}

	frontVelocityReadBuffer[2] = new FLOAT[(localSize[0]) * (localSize[1])];
	backVelocityReadBuffer[2] = new FLOAT[(localSize[0]) * (localSize[1])];
	for (int i = 0; i < dim - 1; i++) {
		frontVelocityReadBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[1] + 1)];
		backVelocityReadBuffer[i] = new FLOAT[(localSize[0] + 1)
				* (localSize[1] + 1)];
	}
}

VelocityBufferReadStencil::~VelocityBufferReadStencil() {
}

void VelocityBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {

	// 	With lowoffset = 1 and highoffset = -1 since (i-1) array
	if ((j >= 2) & (k >= 2)) {
		leftVelocityReadBuffer[0][(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i - 1, j, k))[0];
		leftVelocityReadBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		leftVelocityReadBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	} else if ((j == 1) & (k >= 2)) {
		leftVelocityReadBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
	} else if ((k == 1) & (j >= 2)) {
		leftVelocityReadBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	}

}
void VelocityBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	// First internal layer of the sub-domain
	// Iterate with lowoffset = 1, and highoffset = -1
	if ((j >= 2) & (k >= 2)) {
		rightVelocityReadBuffer[0][(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[0];
		rightVelocityReadBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
		rightVelocityReadBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[2];
	} else if (j == 1) {
		rightVelocityReadBuffer[1][(j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
	} else if (k == 1) {
		rightVelocityReadBuffer[2][(j - 2) + (k - 1) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[2];
	}

}
void VelocityBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {

	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		bottomVelocityReadBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		bottomVelocityReadBuffer[1][(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j - 1, k))[1];
		bottomVelocityReadBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		bottomVelocityReadBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		bottomVelocityReadBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[2];
	}

}
void VelocityBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {

	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		topVelocityReadBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[0];
		topVelocityReadBuffer[1][(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[1];
		topVelocityReadBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		topVelocityReadBuffer[0][(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		topVelocityReadBuffer[2][(k - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[2];
	}

}
void VelocityBufferReadStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {

	// Remains the same with the lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		frontVelocityReadBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		frontVelocityReadBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		frontVelocityReadBuffer[2][(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k - 1))[2];
	} else if ((i == 1) & (j >= 2)) {
		frontVelocityReadBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((j == 1) & (i >= 2)) {
		frontVelocityReadBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
	}
}
void VelocityBufferReadStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	// lowoffset = 1 ands highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		backVelocityReadBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[0];
		backVelocityReadBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[1];
		backVelocityReadBuffer[2][(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[2];
	} else if ((i == 1) & (j >= 2)) {
		backVelocityReadBuffer[0][(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[0];
	} else if ((j == 1) & (i >= 2)) {
		backVelocityReadBuffer[1][(j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[1];
	}
}

void VelocityBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void VelocityBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}

