/*
 * PressureBufferReadStencil.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: master
 */

#include "PressureBufferReadStencil.h"

PressureBufferReadStencil::PressureBufferReadStencil(Parameters &parameters) :
		BoundaryStencil<FlowField>(parameters), localSize(
				parameters.parallel.localSize) {

	// Giving the max memory corresponding to the points
	leftPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
	rightPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

	bottomPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
	topPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

	frontPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
	backPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}

PressureBufferReadStencil::~PressureBufferReadStencil() {
}

void PressureBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.leftNb >= 0){
		// 	With lowoffset = 1 and highoffset = -1 since (i-1) array
		if ((j >= 2) & (k >= 2)) {

			(flowField.getPressure().getScalar(i, j, k)) = leftPressureReadBuffer[(j
					- 2) + (k - 2) * localSize[1]];

		}
	}
}
void PressureBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.rightNb >= 0){
		// First internal layer of the sub-domain
		// Iterate with lowoffset = 1, and highoffset = -1
		if ((j >= 2) & (k >= 2)) {

			(flowField.getPressure().getScalar(i + 1, j, k)) =
					rightPressureReadBuffer[(j - 2) + (k - 2) * localSize[1]];
		}
	}
}
void PressureBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.bottomNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (k >= 2)) {
			(flowField.getPressure().getScalar(i, j, k)) =
					bottomPressureReadBuffer[(i - 2) * localSize[2] + (k - 2)];
		}
	}
}
void PressureBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.topNb >= 0){
		// With normal lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (k >= 2)) {

			(flowField.getPressure().getScalar(i, j + 1, k)) =
					topPressureReadBuffer[(i - 2) * localSize[2] + (k - 2)];
		}
	}


}
void PressureBufferReadStencil::applyFrontWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.frontNb >= 0){
		// Remains the same with the lowoffset = 1 and highoffset = -1
		if ((i >= 2) & (j >= 2)) {

			(flowField.getPressure().getScalar(i, j, k)) =
					frontPressureReadBuffer[(i - 2) * localSize[1] + (j - 2)];
		}
	}

}
void PressureBufferReadStencil::applyBackWall(FlowField & flowField, int i,
		int j, int k) {
	if( _parameters.parallel.backNb >= 0){
		// lowoffset = 1 ands highoffset = -1
		if ((i >= 2) & (j >= 2)) {

			(flowField.getPressure().getScalar(i, j, k + 1)) =
					backPressureReadBuffer[(i - 2) * localSize[1] + (j - 2)];
		}
	}
}

void PressureBufferReadStencil::applyLeftWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferReadStencil::applyRightWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferReadStencil::applyBottomWall(FlowField & flowField, int i,
		int j) {
}

void PressureBufferReadStencil::applyTopWall(FlowField & flowField, int i,
		int j) {
}

