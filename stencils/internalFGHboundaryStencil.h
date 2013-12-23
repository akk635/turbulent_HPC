/*
 * internalFGHboundaryStencil.h
 *
 *  Created on: Dec 22, 2013
 *      Author: karthik
 */

#ifndef INTERNALFGHBOUNDARYSTENCIL_H_
#define INTERNALFGHBOUNDARYSTENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include "../GlobalBoundaryFactory.h"
#include "StencilFunctions.h"

/*void loadLocalVelocity3D(FlowField & flowField, FLOAT * const localVelocity, int i, int j, int k);
inline FLOAT computeF3D(const FLOAT * const localVelocity, const Parameters & parameters, FLOAT dt);
inline FLOAT computeG3D(const FLOAT * const localVelocity, const Parameters & parameters, FLOAT dt);
inline FLOAT computeH3D(const FLOAT * const localVelocity, const Parameters & parameters, FLOAT dt);*/


class internalFGHboundaryStencil:  protected BoundaryStencil<FlowField>, public GlobalBoundaryFactory{

private:
	// Exactly like the FGH Stencil
    FLOAT _localVelocity [ 27 * 3 ];
public:

	internalFGHboundaryStencil( Parameters & parameters );
	~internalFGHboundaryStencil(){};

	void applyLeftWall   ( FlowField & flowField, int i, int j ){};
	void applyRightWall  ( FlowField & flowField, int i, int j ){};
	void applyBottomWall ( FlowField & flowField, int i, int j ){};
	void applyTopWall    ( FlowField & flowField, int i, int j ){};

    void applyLeftWall   ( FlowField & flowField, int i, int j, int k );
    void applyRightWall  ( FlowField & flowField, int i, int j, int k );
    void applyBottomWall ( FlowField & flowField, int i, int j, int k );
    void applyTopWall    ( FlowField & flowField, int i, int j, int k ){};
    void applyFrontWall  ( FlowField & flowField, int i, int j, int k );
    void applyBackWall   ( FlowField & flowField, int i, int j, int k ){};

};



#endif /* INTERNALFGHBOUNDARYSTENCIL_H_ */
