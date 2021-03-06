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

	leftVelocityBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
			+ ((localSize[1] + 1) * (localSize[2]))
			+ (localSize[1] * (localSize[2] + 1))];
	rightVelocityBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
			+ ((localSize[1] + 1) * (localSize[2]))
			+ (localSize[1] * (localSize[2] + 1))];

	bottomVelocityBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
			+ ((localSize[0] + 1) * (localSize[2]))
			+ ((localSize[0]) * (localSize[2] + 1))];
	topVelocityBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
			+ ((localSize[0] + 1) * (localSize[2]))
			+ ((localSize[0]) * (localSize[2] + 1))];

	frontVelocityBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
			+ ((localSize[0]) * (localSize[1] + 1))
			+ ((localSize[0] + 1) * (localSize[1]))];
	backVelocityBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
			+ ((localSize[0]) * (localSize[1] + 1))
			+ ((localSize[0] + 1) * (localSize[1]))];

}

VelocityBufferFillStencil::~VelocityBufferFillStencil() {
}

void VelocityBufferFillStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
/*	counter[0]++;*/
	// std::cout<<" i : " << i << " j : "<< j << " k : "<<k<<std::endl;
	// First internal layer of the sub-domain
	// Iterate with lowoffset = 1, highoffset = -1 while i = 2 is the subdomian boundary so (i+1) in the vector
	if ((j >= 2) & (k >= 2)) {
		leftVelocityBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i + 1, j, k))[0];
		leftVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ (j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
		leftVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
				+ (k - 1) * localSize[1]] = (flowField.getVelocity().getVector(
				i + 1, j, k))[2];
	} else if ((j == 1) & (k >= 2)) {
		leftVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ (j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i + 1, j, k))[1];
	} else if ((k == 1) & (j >= 2)) {
		leftVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
				+ (k - 1) * localSize[1]] = (flowField.getVelocity().getVector(
				i + 1, j, k))[2];
	}
}
void VelocityBufferFillStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	// 	With lowoffset = 1 and highoffset = -1 since i = cellsX - 1 + highoffset
	if ((j >= 2) & (k >= 2)) {
		rightVelocityBuffer[(j - 2) + (k - 2) * localSize[1]] =
				(flowField.getVelocity().getVector(i - 1, j, k))[0];
		rightVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ (j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		rightVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
				+ (k - 1) * localSize[1]] = (flowField.getVelocity().getVector(
				i, j, k))[2];
	} else if ((j == 1) & (k >= 2)) {
		rightVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ (j - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
	} else if ((k == 1) & (j >= 2)) {
		rightVelocityBuffer[((localSize[1]) * (localSize[2]))
				+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
				+ (k - 1) * localSize[1]] = (flowField.getVelocity().getVector(
				i, j, k))[2];
	}
}
void VelocityBufferFillStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1 and highoffset = -1 with (j+1) in the vector access
	if ((i >= 2) & (k >= 2)) {
		bottomVelocityBuffer[(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[0];
		bottomVelocityBuffer[((localSize[0] + 1) * (localSize[2] ))
				+ (i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[1];
		bottomVelocityBuffer[((localSize[0] + 1) * (localSize[2]))
				+ ((localSize[0]) * (localSize[2])) + (k - 1) * localSize[0]
				+ (i - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		bottomVelocityBuffer[(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		bottomVelocityBuffer[((localSize[0] + 1) * (localSize[2]))
				+ ((localSize[0]) * (localSize[2])) + (k - 1) * localSize[0]
				+ (i - 2)] =
				(flowField.getVelocity().getVector(i, j + 1, k))[2];
	}

}
void VelocityBufferFillStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	// With normal lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (k >= 2)) {
		topVelocityBuffer[(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		topVelocityBuffer[((localSize[0] + 1) * (localSize[2]))
				+ (i - 2) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j - 1, k))[1];
		topVelocityBuffer[((localSize[0] + 1) * (localSize[2] ))
				+ ((localSize[0]) * (localSize[2])) + (k - 1) * localSize[0]
				+ (i - 2)] = (flowField.getVelocity().getVector(i, j, k))[2];
	} else if ((i == 1) & (k >= 2)) {
		topVelocityBuffer[(i - 1) * localSize[2] + (k - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((k == 1) & (i >= 2)) {
		topVelocityBuffer[((localSize[0] + 1) * (localSize[2]))
				+ ((localSize[0]) * (localSize[2])) + (k - 1) * localSize[0]
				+ (i - 2)] = (flowField.getVelocity().getVector(i, j, k))[2];
	}
}
void VelocityBufferFillStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	// With lowoffset = 1, highoffset = -1 and (k+1) vector access
	if ((i >= 2) & (j >= 2)) {
		frontVelocityBuffer[(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[0];
		frontVelocityBuffer[((localSize[0] + 1) * (localSize[1]))
				+ (j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[1];
		frontVelocityBuffer[ ((localSize[0]) * (localSize[1] + 1)) + ((localSize[0] + 1) * (localSize[1] ))
				+ (i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[2];
	} else if ((i == 1) & (j >= 2)) {
		frontVelocityBuffer[(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[0];
	} else if ((j == 1) & (i >= 2)) {
		frontVelocityBuffer[((localSize[0] + 1) * (localSize[1]))
				+ (j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k + 1))[1];
	}
}
void VelocityBufferFillStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	// Remains the same with the lowoffset = 1 and highoffset = -1
	if ((i >= 2) & (j >= 2)) {
		backVelocityBuffer[(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
		backVelocityBuffer[((localSize[0] + 1) * (localSize[1]))
				+ (j - 1) * localSize[0] + (i - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[1];
		backVelocityBuffer[((localSize[0]) * (localSize[1] + 1)) + ((localSize[0] + 1) * (localSize[1] ))
				+ (i - 2) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k - 1))[2];
	} else if ((i == 1) & (j >= 2)) {
		backVelocityBuffer[(i - 1) * localSize[1] + (j - 2)] =
				(flowField.getVelocity().getVector(i, j, k))[0];
	} else if ((j == 1) & (i >= 2)) {
		backVelocityBuffer[((localSize[0] + 1) * (localSize[1]))
				+ (j - 1) * localSize[0] + (i - 2)] =
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

