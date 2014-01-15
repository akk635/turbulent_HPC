#include "TurbulentViscosityStencil.h"

TurbulentViscosityStencil::TurbulentViscosityStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}


void TurbulentViscosityStencil::apply ( FlowField & flowField, int i, int j ) {
    flowField.getViscosity().getScalar (i, j) = 0;
}


void TurbulentViscosityStencil::apply ( FlowField & flowField, int i, int j, int k ) {
	if (flowField.getFlags().getValue(i,j,k) == 0){
		flowField.getViscosity().getScalar (i, j, k) = 0;
	}
}
