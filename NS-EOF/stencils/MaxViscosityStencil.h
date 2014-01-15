#ifndef _MAX_Viscosity_STENCIL_H_
#define _MAX_Viscosity_STENCIL_H_

#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"


class MaxViscosityStencil : public FieldStencil<FlowField> {

    private:

        FLOAT _maxValues;  //! Stores the maximum module of every component

        /** Sets the maximum value arrays to the value of the cell if it surpasses the current one.
         *
         * 2D version of the function
         * @param flowField Flow field
         * @param i Position in the X direction.
         * @param j Position in the Y direction.
         */
        void cellMaxValue(FlowField & flowField, int i, int j);

        /** Sets the maximum value arrays to the value of the cell if it surpasses the current one.
         *
         * 3D version of the function
         * @param flowField Flow field
         * @param i Position in the X direction.
         * @param j Position in the Y direction.
         * @param k Position in the Z direction.
         */
        void cellMaxValue(FlowField & flowField, int i, int j, int k);

    public:

        /** Constructor
         *
         * @param parameters Parameters of the problem
         */
        MaxViscosityStencil (const Parameters & parameters);

        //@ brief Body iterations
        //@{
        void apply (FlowField & flowField, int i, int j);
        void apply (FlowField & flowField, int i, int j, int k);
        //@}

        /** Resets the maximum values to zero before computing the timestep
         */
        void reset ();

        /** Returns the array with the maximum modules of the components of the velocity
         */
        const FLOAT getMaxValues() const;
};

#endif
