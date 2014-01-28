/*
 * InitBoundLayerThicknessStencil.h
 *
 *  Created on: Jan 14, 2014
 *      Author: Valizadeh
 */

#ifndef INITBOUNDLAYERTHICKNESSSTENCIL_H_

#define INITBOUNDLAYERTHICKNESSSTENCIL_H_

#include "../Parameters.h"
#include "../DataStructures.h"
#include "../FlowField.h"
#include "../Stencil.h"
#include <math.h>

class InitBoundLayerThicknessStencil: public FieldStencil<FlowField> {

	public:
		InitBoundLayerThicknessStencil (const Parameters & parameters): FieldStencil<FlowField> ( parameters ){}

		//~InitBoundLayerThicknessStencil(){}

		// For initialising the 2d velocity fields
		void apply ( FlowField & flowField, int i, int j){

		}

		// For initialising the 3d velocity fields
		void apply ( FlowField & flowField, int i, int j, int k ) {
			//
			if (_parameters.turbulent.turbulent_scenario == "turbulent_laminar_plate"){
				flowField.getDelta().getScalar (i, j, k) = 4.91 * pow( (_parameters.geometry.dx * (_parameters.parallel.firstCorner[0] + i) ) , 0.5) / pow( (_parameters.flow.Re ), 0.5);
			}
			else if (_parameters.turbulent.turbulent_scenario == "turbulent_turbulent_plate"){
				flowField.getDelta().getScalar (i, j, k) = 0.382 * pow( (_parameters.geometry.dx * (_parameters.parallel.firstCorner[0] + i) ) , 0.8) / pow( (_parameters.flow.Re ), 0.2);
			}
		}
};


#endif /* INITBOUNDLAYERTHICKNESSSTENCIL_H_ */
