#include "MaxViscosityStencil.h"
#include <algorithm>
#include <math.h>


MaxViscosityStencil::MaxViscosityStencil (const Parameters & parameters) :
    FieldStencil<FlowField> (parameters), BoundaryStencil<FlowField> (parameters) {
    reset();
}

void MaxViscosityStencil::apply (FlowField & flowField, int i, int j){
    cellMaxValue(flowField, i, j);
}

void MaxViscosityStencil::apply (FlowField & flowField, int i, int j, int k){
    cellMaxValue(flowField, i, j, k);
}

void MaxViscosityStencil::cellMaxValue(FlowField & flowField, int i, int j){
    FLOAT Viscosity = flowField.getViscosity().getScalar(i, j);
    if (fabs(Viscosity) > _maxValues){
        _maxValues = fabs(Viscosity);
    }
}

void MaxUStencil::cellMaxValue(FlowField & flowField, int i, int j, int k){
    FLOAT Viscosity = flowField.getViscosity().getScalar(i, j, k);
    if (fabs(Viscosity) > _maxValues){
        _maxValues = fabs(Viscosity);
    }
}

void MaxUStencil::reset () {
    _maxValues = 0;
}

const FLOAT MaxViscosityStencil::getMaxValues() const{
    return _maxValues;
}
