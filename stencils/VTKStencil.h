#ifndef _VTK_STENCIL_H_
#define _VTK_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <string>
#include <fstream>
#include <sstream>

/** TODO WORKSHEET 1: Stencil for writting VTK files
 *
 */
class VTKStencil : public FieldStencil<FlowField> {

    public:
		int lastCorner[3];
		int firstCorner[3];
        /** Constructor
         *
         */
        VTKStencil ( const Parameters & parameters ): FieldStencil<FlowField>(parameters){
          // TODO WORKSHEET 1
        	firstCorner = parameters.parallel.firstCorner;
        	int *localSize = parameters.parallel.localSize;
        	for ( int i = 0; i < 3; i++ ){
        		lastCorner[i] = firstCorner[i] + localSize[i];
        	}
        }

        /** 2D operation for one position
         *
         * @param flowField State of the flow field
         * @param i Position in the x direction
         * @param j Position in the y direction
         */
        void apply ( FlowField & flowField, int i, int j ){
          // TODO WORKSHEET 1: extra exercise (otherwise leave blank)
        }

        /** 3D operation for one position
         *
         * @param flowField State of the flow field
         * @param i Position in the x direction
         * @param j Position in the y direction
         * @param k Position in the z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k ){
          // TODO WORKSHEET 1
        	//Writng into the vtk file
        	for ( int i = firstCorner[0]; i <= lastCorner[0]; i++ ){
        		for ( int j = firstCorner[1]; j <= lastCorner[1]; j++ ){
        			for ( int k = firstCorner[2]; k <= lastCorner[2]; k++ ){
        				// Values to be written into vtk file
        				flowField.getPressure().getScalar( i, j, k);
        				// It gets messy in 3D for the velocity field
        				// Direct vector addition doesn't work
        				flowField.getVelocity().getVector( i, j, k);
        				flowField.getFlags().getValue( i, j, k );
        			}
        		}
        	}

        }


        /** Writes the information to the file
         * @param flowField Flow field to be written
         * @param timeStep Current time step as integer
         */
        void write ( FlowField & flowField, int timeStep ){
          // TODO WORKSHEET 1
        }

};

#endif
