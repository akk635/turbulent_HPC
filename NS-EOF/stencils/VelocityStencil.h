#ifndef _VELOCITY_STENCIL_H_
#define _VELOCITY_STENCIL_H_

#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"
#include "../GlobalBoundaryFactory.h"


/** Stencil to compute the velocity once the pressure has been found.
 */
class VelocityStencil : public FieldStencil<FlowField>, public BoundaryStencil<FlowField> , protected GlobalBoundaryFactory {

private:

    public:

        /** Constructor
         * @param parameters Parameters of the problem
         */
        VelocityStencil(Parameters & parameters);

        ~VelocityStencil(){};

        /** Apply the stencil in 2D
         * @param flowField Flow field information
         * @param i Position in the X direction
         * @param j Position in the Y direction
         */
        void apply ( FlowField & flowField, int i, int j );

        /** Apply the stencil in 3D
         * @param flowField Flow field information
         * @param i Position in the X direction
         * @param j Position in the Y direction
         * @param k Position in the Z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k );

        /** Represents an operation in the left wall of a 2D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         */
        void applyLeftWall   (FlowField & flowField, int i, int j){};

        /** Represents an operation in the right wall of a 2D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         */
        void applyRightWall  (FlowField & flowField, int i, int j){};

        /** Represents an operation in the bottom wall of a 2D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         */
        void applyBottomWall (FlowField & flowField, int i, int j){};

        /** Represents an operation in the top wall of a 2D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         */
        void applyTopWall    (FlowField & flowField, int i, int j){};


        /** Represents an operation in the left wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
void applyLeftWall   (FlowField & flowField, int i, int j, int k);

        /** Represents an operation in the right wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
void applyRightWall  (FlowField & flowField, int i, int j, int k);

        /** Represents an operation in the bottom wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
void applyBottomWall (FlowField & flowField, int i, int j, int k);

        /** Represents an operation in the top wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
 void applyTopWall    (FlowField & flowField, int i, int j, int k);

        /** Represents an operation in the front wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
void applyFrontWall  (FlowField & flowField, int i, int j, int k);

        /** Represents an operation in the back wall of a 3D domain.
         *
         * @param flowField State of the flow field
         * @param i Index in the x direction
         * @param j Index in the y direction
         * @param k Index in the z direction
         */
void applyBackWall   (FlowField & flowField, int i, int j, int k);

};

#endif
