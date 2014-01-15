#include "TurbulentViscosityStencil.h"
#include <math.h>

TurbulentViscosityStencil::TurbulentViscosityStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}


void TurbulentViscosityStencil::apply ( FlowField & flowField, int i, int j ) {
}


void TurbulentViscosityStencil::apply ( FlowField & flowField, int i, int j, int k ) {

	FLOAT mixing_length;
	//calculating Mixing length
	mixing_length = FieldStencil<FlowField> ::_parameters.turbulent.kappa * flowField.getDistanceToNearWall().getScalar(i,j,k);
	if(FieldStencil<FlowField> ::_parameters.turbulent.turbulent_scenario == "turbulent_laminar_plate" ||
			FieldStencil<FlowField> ::_parameters.turbulent.turbulent_scenario == "turbulent_turbulent_plate" ){
				if ( 0.09*flowField.getDelta().getScalar(i,j,k) < mixing_length ){
					mixing_length = 0.09*flowField.getDelta().getScalar(i,j,k);
				}
	}

	//calculating shear stress term Sij*Sij
	FLOAT Sij2,S11,S22,S33,S12,S13,S23;

	S11 = (flowField.getVelocity().getVector(i,j,k)[0]-flowField.getVelocity().getVector(i-1,j,k)[0])/FieldStencil<FlowField> ::_parameters.geometry.dx;
	S22 = (flowField.getVelocity().getVector(i,j,k)[1]-flowField.getVelocity().getVector(i,j-1,k)[1])/FieldStencil<FlowField> ::_parameters.geometry.dy;
	S33 = (flowField.getVelocity().getVector(i,j,k)[2]-flowField.getVelocity().getVector(i,j,k-1)[2])/FieldStencil<FlowField> ::_parameters.geometry.dz;

	S12 = 0.5 * ( (flowField.getVelocity().getVector(i,j,k)[0]-flowField.getVelocity().getVector(i,j-1,k)[0])/FieldStencil<FlowField> ::_parameters.geometry.dy+
				  (flowField.getVelocity().getVector(i,j,k)[1]-flowField.getVelocity().getVector(i-1,j,k)[1])/FieldStencil<FlowField> ::_parameters.geometry.dx);

	S13 = 0.5 * ( (flowField.getVelocity().getVector(i,j,k)[0]-flowField.getVelocity().getVector(i,j,k-1)[0])/FieldStencil<FlowField> ::_parameters.geometry.dz+
					  (flowField.getVelocity().getVector(i,j,k)[2]-flowField.getVelocity().getVector(i-1,j,k)[2])/FieldStencil<FlowField> ::_parameters.geometry.dx);

	S23 = 0.5 * ( (flowField.getVelocity().getVector(i,j,k)[1]-flowField.getVelocity().getVector(i,j,k-1)[1])/FieldStencil<FlowField> ::_parameters.geometry.dz+
						  (flowField.getVelocity().getVector(i,j,k)[2]-flowField.getVelocity().getVector(i,j-1,k)[2])/FieldStencil<FlowField> ::_parameters.geometry.dy);

	Sij2=S11*S11 + S22*S22 + S33*S33 + 2 * ( S12*S12 + S13*S13 + S23*S23 );

	//calculating turbulent viscosities
	if (flowField.getFlags().getValue(i,j,k) == 0){
		flowField.getViscosity().getScalar (i, j, k) = mixing_length*mixing_length*sqrt( 2*Sij2 );
	}

}
