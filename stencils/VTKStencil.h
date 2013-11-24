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
		std::ofstream fpV;
		std::ofstream fpP;
		const int *firstCorner;
		const int *localSize;
    	std::string vtkVelocity;
    	std::string vtkPressure;
    	FLOAT *const velocity = new FLOAT[3];

    public:
        /** Constructor
         *
         */
        VTKStencil ( const Parameters & parameters ): FieldStencil<FlowField>(parameters) {
          // TODO WORKSHEET 1

        	_dim = parameters.geometry.dim;
        	firstCorner = NULL;
        	firstCorner = parameters.parallel.firstCorner;
        	localSize = NULL;
        	localSize = parameters.parallel.localSize;
        	vtkVelocity = parameters.vtk.prefix;
        	vtkPressure = parameters.vtk.prefix;
        }

        // default destructor
        ~VTKStencil() {
        	firstCorner = NULL;
        	localSize = NULL;
        	fpV.close();
        	fpP.close();
        	delete[] velocity;
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
        	if( flowField.getFlags().getValue(i,j,k) == 0 ){
        		flowField.getPressureAndVelocity( flowField.getPressure().getScalar(i, j, k), velocity, i, j, k);

        		fpV << velocity[0] << " " << velocity[1] <<
        				" " << velocity[2] << "\n" ;
        		fpP << flowField.getPressure().getScalar(i, j, k) << "\n";
        	}
        	else {
        		fpV << (FLOAT) 0.0 << " " << (FLOAT) 0.0 << " " << (FLOAT) 0.0 << "\n" ;
        		fpP << (FLOAT) 0.0 << "\n";
        	}

        }


        /** Writes the information to the file
         * @param flowField Flow field to be written
         * @param timeStep Current time step as integer
         */
        void write ( FlowField & flowField, int timeStep ){
          // TODO WORKSHEET 1
        	// the apply method itself writes the files into apply
        	vtkVelocity += "_velocity_";
        	vtkPressure += "_pressure_";
        	std::stringstream ss;
        	ss<<timeStep;
        	vtkVelocity += ( ss.str()+".vtk" );
        	vtkPressure += ( ss.str()+".vtk" );

        	fpV.open( vtkVelocity.c_str() );
        	fpP.open( vtkPressure.c_str() );

        	write_vtkHeader( fpV, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], _parameters.geometry.dx, _parameters.geometry.dy,
        	        		_parameters.geometry.dz);

        	write_vtkHeader( fpP, localSize[0], localSize[1],
        					localSize[2], firstCorner[0], firstCorner[1],
        					firstCorner[2], _parameters.geometry.dx, _parameters.geometry.dy,
        	        		_parameters.geometry.dz);

        	fpV << "POINT_DATA " << ((localSize[0]+1) * (localSize[1]+1) * (localSize[2]+1)) << "\n";
        	// fpV << "CELL_DATA " <<  (localSize[0] * localSize[1] * localSize[2]) << "\n" ;
        	fpV << "VECTORS velocity double \n" ;
        	fpP << "CELL_DATA " <<  (localSize[0] * localSize[1] * localSize[2]) << "\n" ;
        	fpP << "SCALARS pressure double 1 \n";
        	fpP << "LOOKUP_TABLE default \n";

        }

};

#endif
