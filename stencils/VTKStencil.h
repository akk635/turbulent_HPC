#ifndef _VTK_STENCIL_H_
#define _VTK_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <string>
#include <fstream>
#include <sstream>
#include "visual.h"

/** TODO WORKSHEET 1: Stencil for writting VTK files
 *
 */
class VTKStencil : public FieldStencil<FlowField> {

	private:
		int _dim;
		FILE *fpV = NULL;
		FILE *fpP = NULL;
		int _timeStep;
    public:
		int lastCorner[3];
		const int *firstCorner;
		FLOAT pressure;
		FLOAT * velocity;
        /** Constructor
         *
         */
        VTKStencil ( const Parameters & parameters ): FieldStencil<FlowField>(parameters) {
          // TODO WORKSHEET 1
        	firstCorner = parameters.parallel.firstCorner;
        	_dim = parameters.geometry.dim;
        	velocity = new FLOAT [_dim];
        	const int *localSize = parameters.parallel.localSize;
        	for ( int i = 0; i < 3; i++ ){
        		lastCorner[i] = firstCorner[i] + localSize[i];
        	}
        	std::string vtkVelocity;
        	vtkVelocity = parameters.vtk.prefix;
        	/*for ( int i=0; i<3; i++){
        		vtkVelocity += (firstCorner[i]+'_');
        	}*/

        	vtkVelocity += "_velocity.vtk";
        	fpV = fopen( vtkVelocity.c_str(), "w");
        	/*write_vtkHeader( fpV, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], parameters.geometry.dx, parameters.geometry.dy,
        	        		parameters.geometry.dz);*/

        	std::string vtkPressure;
        	vtkPressure = parameters.vtk.prefix;
        	vtkPressure += "_pressure.vtk";
        	fpP = fopen( vtkPressure.c_str(), "w");
        	/*write_vtkHeader( fpP, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], parameters.geometry.dx, parameters.geometry.dy,
        	        		parameters.geometry.dz);*/


        	fprintf(fpV,"POINT_DATA %i \n",  localSize[0] * localSize[1] * localSize[2] );
        	fprintf(fpV, "VECTORS velocity float \n");
        	fprintf(fpP,"POINT_DATA %i \n",  localSize[0] * localSize[1] * localSize[2] );
        	fprintf(fpP, "SCALARS pressure float 1 \n");

        	// For the case of 3D
/*        	if( localSize[2] != 0 ){
        		_dim = 3;
        		pressure = new FLOAT[ localSize[0] * localSize[1] * localSize[2] ];
        		velocity = new FLOAT[ localSize[0] * localSize[1] * localSize[2] * 3 ];
           	} else {
           		_dim = 2;
        		pressure = new FLOAT[ localSize[0] * localSize[1] ];
        		velocity = new FLOAT[ localSize[0] * localSize[1] * 3 ];
           	}
           	*/
        }

        // default destructor
        ~VTKStencil() {
        	delete[] velocity;
        	fclose( fpV );
        	fclose( fpP );
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
        	// Writing the vtk file from the local values
        	if( flowField.getFlags().getValue(i,j,k) ){
        		flowField.getPressureAndVelocity( pressure, velocity, i, j, k);
        		fprintf( fpP, "%f %f %f \n", velocity[0], velocity[1], velocity[2] );
        		fprintf( fpP, "%f \n", pressure);
        	}
        	else {
        		fprintf( fpP, "%f %f %f \n", 0.0,0.0,0.0 );
        		fprintf( fpP, "%f \n", 0.0);
        	}

        }


        /** Writes the information to the file
         * @param flowField Flow field to be written
         * @param timeStep Current time step as integer
         */
        void write ( FlowField & flowField, int timeStep ){
          // TODO WORKSHEET 1
        	// the apply method itself writes the files into apply
        	_timeStep = timeStep;
        }

};

#endif
