/*
 * visual.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: karthik
 */
#include "helper.h"
#include "visual.h"
#include <stdio.h>

// Method with both the points and the header
void write_vtkHeader( std::ofstream &fp, int localsizeX, int localsizeY,
				int localsizeZ, int originX, int originY,
				int originZ, FLOAT dx, FLOAT dy,
        		FLOAT dz) {
	  if( fp == NULL )
	  {
	    std::string szBuff;
	    szBuff = "Null pointer in write_vtkHeader";
	    handleError(1, "please pass a valid vtk filename for printing \n");
	  }
	  fp << "# vtk DataFile Version 2.0 \n";
	  fp << "Visualization \n";
	  fp << "ASCII \n";
	  fp << "DATASET STRUCTURED_GRID \n";
	  fp << "DIMENSIONS"<<" "<<(localsizeX + 1)<<" "<<(localsizeY + 1)<<" "<<(localsizeZ + 1) <<"\n";
	  fp << "POINTS " << ((localsizeX + 1) * (localsizeY + 1) * (localsizeZ + 1)) << " float" << "\n";

	  for (int k=0; k < ( localsizeZ+1 ); k++){
		  for (int j=0; j < ( localsizeY+1 ); j++){
			  for (int i=0; i < ( localsizeX+1 ); i++){
				  fp << (float)(originX * dx + ((i)*dx)) << " " << (float) (originY * dy + ((j)*dy)) << " " << (float) (originZ * dz + ((k)*dz)) << "\n";
			  }
		  }
	  }
}






