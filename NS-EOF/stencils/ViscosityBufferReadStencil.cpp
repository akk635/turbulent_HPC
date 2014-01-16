/*
 * ViscosityBufferReadStencil.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: mahyar
 */

#include "ViscosityBufferReadStencil.h"

ViscosityBufferReadStencil::ViscosityBufferReadStencil(Parameters &parameters) :
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {

	// Giving the max memory corresponding to the points
	leftViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
	rightViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

	bottomViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
	topViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

	frontViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
	backViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}

ViscosityBufferReadStencil::~ViscosityBufferReadStencil() {
}

void ViscosityBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.leftNb >= 0){
		// 	With lowoffset = 1 and highoffset = -1 since (i-1) array
		if ((j >= 2) & (k >= 2)) {

			(flowField.getViscosity().getScalar(i, j, k)) = leftViscosityReadBuffer[(j
					- 2) + (k - 2) * localSize[1]];

		}
	}
}
void ViscosityBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.rightNb >= 0){
		// First internal layer of the sub-domain
		// Iterate with lowoffset = 1, and highoffset = -1
		if ((j >= 2) & (k >= 2)) {

			(flowField.getViscosity().getScalar(i + 1, j, k)) =
					rightViscosityReadBuffer[(j - 2) + (k - 2) * localSize[1]];
		}
	}
}
void ViscosityBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.bottomNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (k >= 2)) {
			(flowField.getViscosity().getScalar(i, j, k)) =
					bottomViscosityReadBuffer[(i - 2) * localSize[2] + (k - 2)];
		}
	}
}
void ViscosityBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.topNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (k >= 2)) {

			(flowField.getViscosity().getScalar(i, j + 1, k)) =
					topViscosityReadBuffer[(i - 2) * localSize[2] + (k - 2)];
		}
	}


}
void ViscosityBufferReadStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.frontNb >= 0){
		// Remains the same with the lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (j >= 2)) {

			(flowField.getViscosity().getScalar(i, j, k)) =
					frontViscosityReadBuffer[(i - 2) * localSize[1] + (j - 2)];
		}
	}

}
void ViscosityBufferReadStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.backNb >= 0){
		// lowoffset = 1 ands highoffset = -1
		if ((i >= 2) & (j >= 2)) {

			(flowField.getViscosity().getScalar(i, j, k + 1)) =
					backViscosityReadBuffer[(i - 2) * localSize[1] + (j - 2)];
		}
	}
}

void ViscosityBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void ViscosityBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}

