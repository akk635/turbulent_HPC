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

	/*	leftVelocityReadBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
	 + 2 * ((localSize[1] + 1) * (localSize[2] + 1))];
	 rightVelocityReadBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
	 + 2 * ((localSize[1] + 1) * (localSize[2] + 1))];

	 bottomVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
	 + 2 * ((localSize[0] + 1) * (localSize[2] + 1))];
	 topVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
	 + 2 * ((localSize[0] + 1) * (localSize[2] + 1))];

	 frontVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
	 + 2 * ((localSize[0] + 1) * (localSize[1] + 1))];
	 backVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
	 + 2 * ((localSize[0] + 1) * (localSize[1] + 1))];*/

	leftVelocityReadBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
			+ ((localSize[1] + 1) * (localSize[2]))
			+ (localSize[1] * (localSize[2] + 1))];
	rightVelocityReadBuffer = new FLOAT[((localSize[1]) * (localSize[2]))
			+ ((localSize[1] + 1) * (localSize[2]))
			+ (localSize[1] * (localSize[2] + 1))];

	bottomVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
			+ ((localSize[0] + 1) * (localSize[2]))
			+ ((localSize[0]) * (localSize[2] + 1))];
	topVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[2]))
			+ ((localSize[0] + 1) * (localSize[2]))
			+ ((localSize[0]) * (localSize[2] + 1))];

	frontVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
			+ ((localSize[0]) * (localSize[1] + 1))
			+ ((localSize[0] + 1) * (localSize[1]))];
	backVelocityReadBuffer = new FLOAT[((localSize[0]) * (localSize[1]))
			+ ((localSize[0]) * (localSize[1] + 1))
			+ ((localSize[0] + 1) * (localSize[1]))];

}

VelocityBufferReadStencil::~VelocityBufferReadStencil() {
}

void VelocityBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
if( _parameters.parallel.leftNb >= 0){
	// 	With lowoffset = 1 and highoffset = -1 since (i-1) array
	if ((j >= 2) & (k >= 2)) {
		(flowField.getVelocity().getVector(i - 1, j, k))[0] =
				leftVelocityReadBuffer[(j - 2) + (k - 2) * localSize[1]];

		(flowField.getVelocity().getVector(i, j, k))[1] =
				leftVelocityReadBuffer[((localSize[1]) * (localSize[2]))
						+ (j - 1) * localSize[2] + (k - 2)];
		(flowField.getVelocity().getVector(i, j, k))[2] =
				leftVelocityReadBuffer[((localSize[1]) * (localSize[2]))
						+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
						+ (k - 1) * localSize[1]];
	} else if ((j == 1) & (k >= 2)) {

		(flowField.getVelocity().getVector(i, j, k))[1] =
				leftVelocityReadBuffer[((localSize[1]) * (localSize[2]))
						+ (j - 1) * localSize[2] + (k - 2)];
	} else if ((k == 1) & (j >= 2)) {
		(flowField.getVelocity().getVector(i, j, k))[2] =
				leftVelocityReadBuffer[((localSize[1]) * (localSize[2]))
						+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
						+ (k - 1) * localSize[1]];
	}
}
}
void VelocityBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.rightNb >= 0){
		// First internal layer of the sub-domain
		// Iterate with lowoffset = 1, and highoffset = -1
			if ((j >= 2) & (k >= 2)) {

				(flowField.getVelocity().getVector(i + 1, j, k))[0] =
						rightVelocityReadBuffer[(j - 2) + (k - 2) * localSize[1]];

				(flowField.getVelocity().getVector(i + 1, j, k))[1] =
						rightVelocityReadBuffer[((localSize[1]) * (localSize[2]))
								+ (j - 1) * localSize[2] + (k - 2)];
				(flowField.getVelocity().getVector(i + 1, j, k))[2] =
						rightVelocityReadBuffer[((localSize[1]) * (localSize[2]))
								+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
								+ (k - 1) * localSize[1]];
			} else if ((j == 1) & (k >= 2)) {
				(flowField.getVelocity().getVector(i + 1, j, k))[1] =
						rightVelocityReadBuffer[((localSize[1]) * (localSize[2]))
								+ (j - 1) * localSize[2] + (k - 2)];
			} else if ((k == 1) & (j >= 2)) {
				(flowField.getVelocity().getVector(i + 1, j, k))[2] =
						rightVelocityReadBuffer[((localSize[1]) * (localSize[2]))
								+ ((localSize[1] + 1) * (localSize[2])) + (j - 2)
								+ (k - 1) * localSize[1]];
			}
	}
}
void VelocityBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.bottomNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
			if ((i >= 2) & (k >= 2)) {
				(flowField.getVelocity().getVector(i, j, k))[0] =
						bottomVelocityReadBuffer[(i - 1) * localSize[2] + (k - 2)];
				(flowField.getVelocity().getVector(i, j - 1, k))[1] =
						bottomVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ (i - 2) * localSize[2] + (k - 2)];
				(flowField.getVelocity().getVector(i, j, k))[2] =
						bottomVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ ((localSize[0]) * (localSize[2]))
								+ (k - 1) * localSize[0] + (i - 2)];
			} else if ((i == 1) & (k >= 2)) {
				(flowField.getVelocity().getVector(i, j, k))[0] =
						bottomVelocityReadBuffer[(i - 1) * localSize[2] + (k - 2)];
			} else if ((k == 1) & (i >= 2)) {
				(flowField.getVelocity().getVector(i, j, k))[2] =
						bottomVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ ((localSize[0]) * (localSize[2]))
								+ (k - 1) * localSize[0] + (i - 2)];
			}

	}

}
void VelocityBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.topNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
			if ((i >= 2) & (k >= 2)) {
				(flowField.getVelocity().getVector(i, j + 1, k))[0] =
						topVelocityReadBuffer[(i - 1) * localSize[2] + (k - 2)];
				(flowField.getVelocity().getVector(i, j + 1, k))[1] =
						topVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ (i - 2) * localSize[2] + (k - 2)];
				(flowField.getVelocity().getVector(i, j + 1, k))[2] =
						topVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ ((localSize[0]) * (localSize[2]))
								+ (k - 1) * localSize[0] + (i - 2)];
			} else if ((i == 1) & (k >= 2)) {
				(flowField.getVelocity().getVector(i, j + 1, k))[0] =
						topVelocityReadBuffer[(i - 1) * localSize[2] + (k - 2)];
			} else if ((k == 1) & (i >= 2)) {
				(flowField.getVelocity().getVector(i, j + 1, k))[2] =
						topVelocityReadBuffer[((localSize[0] + 1) * (localSize[2]))
								+ ((localSize[0]) * (localSize[2]))
								+ (k - 1) * localSize[0] + (i - 2)];
			}

	}

}
void VelocityBufferReadStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.frontNb >= 0 ){
		// Remains the same with the lowoffset = 1 and highoffset = -1
			if ((i >= 2) & (j >= 2)) {
				(flowField.getVelocity().getVector(i, j, k))[0] =
						frontVelocityReadBuffer[(i - 1) * localSize[1] + (j - 2)];

				(flowField.getVelocity().getVector(i, j, k))[1] =
						frontVelocityReadBuffer[((localSize[0] + 1) * (localSize[1]))
								+ (j - 1) * localSize[0] + (i - 2)];

				(flowField.getVelocity().getVector(i, j, k - 1))[2] =
						frontVelocityReadBuffer[((localSize[0]) * (localSize[1] + 1))
								+ ((localSize[0] + 1) * (localSize[1]))
								+ (i - 2) * localSize[1] + (j - 2)];
			} else if ((i == 1) & (j >= 2)) {
				(flowField.getVelocity().getVector(i, j, k))[0] =
						frontVelocityReadBuffer[(i - 1) * localSize[1] + (j - 2)];
			} else if ((j == 1) & (i >= 2)) {

				(flowField.getVelocity().getVector(i, j, k))[1] =
						frontVelocityReadBuffer[((localSize[0] + 1) * (localSize[1]))
								+ (j - 1) * localSize[0] + (i - 2)];
			}
	}
}

void VelocityBufferReadStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.backNb >= 0){
		// lowoffset = 1 ands highoffset = -1
			if ((i >= 2) & (j >= 2)) {

				(flowField.getVelocity().getVector(i, j, k + 1))[0] =
						backVelocityReadBuffer[(i - 1) * localSize[1] + (j - 2)];

				(flowField.getVelocity().getVector(i, j, k + 1))[1] =
						backVelocityReadBuffer[((localSize[0] + 1) * (localSize[1]))
								+ (j - 1) * localSize[0] + (i - 2)];
				(flowField.getVelocity().getVector(i, j, k + 1))[2] =
						backVelocityReadBuffer[((localSize[0]) * (localSize[1] + 1))
												+ ((localSize[0] + 1) * (localSize[1]))
												+ (i - 2) * localSize[1] + (j - 2)];
			} else if ((i == 1) & (j >= 2)) {

				(flowField.getVelocity().getVector(i, j, k + 1))[0] =
						backVelocityReadBuffer[(i - 1) * localSize[1] + (j - 2)];
			} else if ((j == 1) & (i >= 2)) {

				(flowField.getVelocity().getVector(i, j, k + 1))[1] =
						backVelocityReadBuffer[((localSize[0] + 1) * (localSize[1]))
								+ (j - 1) * localSize[0] + (i - 2)];
			}
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

