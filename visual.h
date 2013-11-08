/*
 * visual.h
 *
 *  Created on: Nov 5, 2013
 *      Author: karthik
 */

#ifndef VISUAL_H_
#define VISUAL_H_

#include "Definitions.h"
#include <fstream>

// With the header and the points in the same thing
void write_vtkHeader( std::ofstream &fp, int localsizeX, int localsizeY,
				int localsizeZ, int originX, int originY,
				int originZ, FLOAT dx, FLOAT dy,
        		FLOAT dz);

#endif /* VISUAL_H_ */
