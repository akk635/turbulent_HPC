#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"
#include <math.h>

class MaxViscosityStencil : public FieldStencil<FlowField> {

private:

	//! Stores the maximum module of every component

public:
	FLOAT _maxValues;

	MaxViscosityStencil(const Parameters & parameters) : FieldStencil<FlowField> (parameters) {}

	void apply (FlowField & flowField, int i, int j){
		if (fabs(flowField.getViscosity().getScalar(i, j)) > _maxValues){
			_maxValues = fabs(flowField.getViscosity().getScalar(i, j));
		}
	}

	void apply (FlowField & flowField, int i, int j, int k){
		if (fabs(flowField.getViscosity().getScalar(i, j, k)) > _maxValues){
			_maxValues = fabs(flowField.getViscosity().getScalar(i, j, k));
		}
	}


	/** Resets the maximum values to zero before computing the timestep
	 */
	void reset () {
		_maxValues = 0;
	}

	/** Returns the array with the maximum modules of the components of the velocity
	 */
	const FLOAT getMaxValues() const{
	    return _maxValues;
	}

};

