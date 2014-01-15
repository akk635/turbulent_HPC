#ifndef _STENCIL_FGH_H_
#define _STENCIL_FGH_H_

#include "../FlowField.h"
#include "../Stencil.h"
#include "../Parameters.h"
#include "../GlobalBoundaryFactory.h"
#include <assert.h>

class FGHStencil : public FieldStencil<FlowField>, public BoundaryStencil<FlowField>, protected GlobalBoundaryFactory
{

    private:

        // A local velocity variable that will be used to approximate derivatives. Size matches 3D
        // case, but can be used for 2D as well.
        FLOAT _localVelocity [ 27 * 3 ];
        // Just the turbulence viscosity
        FLOAT _localViscosity [ 27 * 3 ];


    public:
        FGHStencil ( Parameters & parameters );

        /** Apply the stencil in 2D
         * 
         * Performs the operation of the stencil in a single position given by the indexes.
         * @param flowField State of the flow
         * @param i Index in the x direction
         * @param j Index in the y direction
         */
        void apply ( FlowField & flowField, int i, int j );

        /** Apply the stencil in 3D
         *
         * @param flowField State of the flow
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k );

    	void applyLeftWall   ( FlowField & flowField, int i, int j ){};
    	void applyRightWall  ( FlowField & flowField, int i, int j ){};
    	void applyBottomWall ( FlowField & flowField, int i, int j ){};
    	void applyTopWall    ( FlowField & flowField, int i, int j ){};

        void applyLeftWall   ( FlowField & flowField, int i, int j, int k );
        void applyRightWall  ( FlowField & flowField, int i, int j, int k );
        void applyBottomWall ( FlowField & flowField, int i, int j, int k );
        void applyTopWall    ( FlowField & flowField, int i, int j, int k );
        void applyFrontWall  ( FlowField & flowField, int i, int j, int k );
        void applyBackWall   ( FlowField & flowField, int i, int j, int k );
};


#endif
