/*
 * ViscosityBufferFillStencil.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: mahyar
 */


#include "ViscosityBufferFillStencil.h"

ViscosityBufferFillStencil::ViscosityBufferFillStencil(Parameters &parameters):
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {

	int dim = parameters.geometry.dim;

	// Giving the max memory corresponding to the points
	leftViscosityBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
	rightViscosityBuffer = new FLOAT[(localSize[1]) * (localSize[2])];


	bottomViscosityBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
	topViscosityBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

	frontViscosityBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
	backViscosityBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}

ViscosityBufferFillStencil::~ViscosityBufferFillStencil(){};

void ViscosityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	// First internal layer of the sub-domain
	// Iterate with lowoffset = 1, highoffset = -1 while i = 2 is the subdomian boundary so (i+1) in the vector
	if ((j >= 2) & (k >= 2)) {
		leftViscosityBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getViscosity().getScalar(i + 1, j, k));
	}
}

void ViscosityBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	// 	With lowoffset = 1 and highoffset = -1 since i = cellsX - 1 + highoffset
	if ((j >= 2) & (k >= 2)) {
		rightViscosityBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getViscosity().getScalar(i, j, k));
	}
}

void ViscosityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1 and highoffset = -1 with (j+1) in the vector access
	if ((i >= 2) & (k >= 2)) {
		bottomViscosityBuffer[(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getViscosity().getScalar(i, j+1, k));
	}
}

void ViscosityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		topViscosityBuffer[(i - 2) * localSize[2] + (k - 2)] =
				(flowField.getViscosity().getScalar(i, j, k));
	}
}

void ViscosityBufferFillStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1, highoffset = -1 and (k+1) vector access
	if ((i >= 2) & (j >= 2)) {
		frontViscosityBuffer[(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getViscosity().getScalar(i, j, k+1));
	}
}
void ViscosityBufferFillStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	// Remains the same with the lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		backViscosityBuffer[(i - 2) * localSize[1] + (j - 2)] =
				(flowField.getViscosity().getScalar(i, j, k));
	}
}

void ViscosityBufferFillStencil::applyTest ( FlowField & flowField, int i, int j, int k ){

}

void ViscosityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}


