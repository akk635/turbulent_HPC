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
		// std::ofstream fpV;
		// std::ofstream fpP;
		FILE *fpV = NULL;
		FILE *fpP = NULL;
		int _timeStep;
		int lastCorner[3];
		const int *firstCorner;
		FLOAT pressure;
		FLOAT * velocity;
		const int *localSize;
    	std::string vtkVelocity;
    	std::string vtkPressure;

    public:
        /** Constructor
         *
         */
        VTKStencil ( const Parameters & parameters ): FieldStencil<FlowField>(parameters) {
          // TODO WORKSHEET 1

        	_dim = parameters.geometry.dim;
        	firstCorner = new int [_dim];
        	firstCorner = parameters.parallel.firstCorner;
        	// Memory for writing the velocity values
        	velocity = new FLOAT [_dim];
        	localSize = new int [_dim];
        	localSize = parameters.parallel.localSize;

        	for ( int i = 0; i < 3; i++ ){
        		lastCorner[i] = firstCorner[i] + localSize[i];
        	}

        	vtkVelocity = parameters.vtk.prefix;
        	vtkPressure = parameters.vtk.prefix;

        }

        // default destructor
        ~VTKStencil() {
        	delete[] velocity;
        	delete[] firstCorner;
        	delete[] localSize;
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

        	vtkVelocity += "_velocity.vtk";
        	fpV = fopen( vtkVelocity.c_str(), "w");
        	write_vtkHeader( fpV, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], _parameters.geometry.dx, _parameters.geometry.dy,
        	        		_parameters.geometry.dz);

        	vtkPressure += "_pressure.vtk";
        	fpP = fopen( vtkPressure.c_str(), "w");
        	if (fpP == NULL){
        		std::cout<<"file didnt open";
        	}
        	std::cout<<"ok";

        	write_vtkHeader( fpP, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], _parameters.geometry.dx, _parameters.geometry.dy,
        	        		_parameters.geometry.dz );


        	fprintf(fpV , "POINT_DATA %i \n",  (localSize[0]+1) * (localSize[1]+1) * (localSize[2]+1) );
        	fprintf(fpV, "VECTORS velocity float \n");
        	fprintf(fpP,"POINT_DATA %i \n",  localSize[0] * localSize[1] * localSize[2] );
        	fprintf(fpP, "SCALARS pressure float 1 \n");
        	fprintf(fpP, "LOOKUP_TABLE default \n");

        }

};

#endif
