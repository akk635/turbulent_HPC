/*
 * MessagePassingConfiguration.cpp
 *
 *  Created on: Dec 21, 2013
 *      Author: karthik
 */
#include "MessagePassingConfiguration.h"
#include <fstream>
#include <sstream>
#include <assert.h>

MessagePassingConfiguration::MessagePassingConfiguration(
		Parameters & parameters, FlowField & flowfield) :
		_parameters(parameters), _flowField(flowfield),
				fillStencil(_parameters), readStencil(_parameters),
				velocityfillIterator(_flowField, _parameters, fillStencil, 1, -1),
				velocityreadIterator(_flowField, _parameters, readStencil, 1, -1),

				fillPStencil(_parameters), readPStencil(_parameters),
				pressurefillIterator(_flowField, _parameters, fillPStencil, 1, -1),
				pressurereadIterator(_flowField, _parameters, readPStencil, 1, -1),

				fillVisStencil(_parameters), readVisStencil(_parameters),
				viscosityfillIterator(_flowField,_parameters,fillVisStencil, 1, -1),
				viscosityreadIterator(_flowField,_parameters,readVisStencil, 1, -1){
}

void MessagePassingConfiguration::communicateVelocity() {

	velocityfillIterator.iterate();

	MPI_Request request[2 * 6];
	MPI_Status status[2 * 6];
	const int * localSize = fillStencil.localSize;

	// Now the actual comm to the left side process and recv from the right
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.leftVelocityBuffer,
			((localSize[1]) * (localSize[2]))
					+ ((localSize[1]) * (localSize[2] + 1))+ ((localSize[1] + 1) * (localSize[2] )), MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 101, PETSC_COMM_WORLD, &(request[0]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.rightVelocityReadBuffer,
			3 *((localSize[1]) * (localSize[2]))
					+ localSize[1] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 101, PETSC_COMM_WORLD, &(request[1]));

	// Send to right and recv from left
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.rightVelocityBuffer,
			3 *((localSize[1]) * (localSize[2]))
								+ localSize[1] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 102, PETSC_COMM_WORLD, &(request[2]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.leftVelocityReadBuffer,
			3 *((localSize[1]) * (localSize[2]))
								+ localSize[1] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 102, PETSC_COMM_WORLD, &(request[3]));

	// Send bottom and recv from top
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.bottomVelocityBuffer,
			3 * ((localSize[0]) * (localSize[2]))
					+ localSize[0] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 103, PETSC_COMM_WORLD, &(request[4]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.topVelocityReadBuffer,
			3 * ((localSize[0]) * (localSize[2]))
								+ localSize[0] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.topNb, 103, PETSC_COMM_WORLD, &(request[5]));


	// Send top and recv from bottom
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.topVelocityBuffer,
			3 * ((localSize[0]) * (localSize[2]))
								+ localSize[0] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.topNb, 104, PETSC_COMM_WORLD, &(request[6]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.bottomVelocityReadBuffer,
			3 * ((localSize[0]) * (localSize[2]))
								+ localSize[0] + localSize[2], MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 104, PETSC_COMM_WORLD, &(request[7]));


	// Send front and recv from back
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.frontVelocityBuffer,
			3 * ((localSize[0]) * (localSize[1]))
								+ localSize[0] + localSize[1], MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 105, PETSC_COMM_WORLD, &(request[8]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.backVelocityReadBuffer,
			3 * ((localSize[0]) * (localSize[1]))
											+ localSize[0] + localSize[1], MY_MPI_FLOAT,
			_parameters.parallel.backNb, 105, PETSC_COMM_WORLD, &(request[9]));


	// Send back and recv from front
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.backVelocityBuffer,
			3 * ((localSize[0]) * (localSize[1]))
											+ localSize[0] + localSize[1], MY_MPI_FLOAT,
			_parameters.parallel.backNb, 106, PETSC_COMM_WORLD, &(request[10]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.frontVelocityReadBuffer,
			3 * ((localSize[0]) * (localSize[1]))
											+ localSize[0] + localSize[1], MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 106, PETSC_COMM_WORLD, &(request[11]));

	for (int i = 0; i < 12; i++) {
		MPI_Wait(&(request[i]), &(status[i]));
	}

	// Writing the buffers into appropriate flowfields
	velocityreadIterator.iterate();
}

void MessagePassingConfiguration::communicatePressure() {
/*	int rank;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	std::ofstream fpp;
	std::stringstream ss;
	ss << rank;
	std::string test = "commP";
	test+=ss.str();
	test+=".dat";
	fpp.open(  test.c_str());*/

	pressurefillIterator.iterate();
	MPI_Request requestP[2 * 6];
	MPI_Status statusP[2 * 6];
	const int * localSize = fillPStencil.localSize;

	// Now the actual comm to the left side process and recv from the right
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.leftPressureBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 201, PETSC_COMM_WORLD, &(requestP[0]));
/*	if(rank == 1){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << fillPStencil.leftPressureBuffer[i] <<"\n";
		}
	}*/
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.rightPressureReadBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 201, PETSC_COMM_WORLD, &(requestP[1]));

	// Send to right and recv from left
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.rightPressureBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 202, PETSC_COMM_WORLD, &(requestP[2]));
/*	if(rank ==0 ){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << fillPStencil.rightPressureBuffer[i] <<"\n";
		}
	}*/
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.leftPressureReadBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 202, PETSC_COMM_WORLD, &(requestP[3]));

	// Send bottom and recv from top
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.bottomPressureBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 203, PETSC_COMM_WORLD, &(requestP[4]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.topPressureReadBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.topNb, 203, PETSC_COMM_WORLD, &(requestP[5]));

	// Send top and recv from bottom
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.topPressureBuffer, ((localSize[0]) * (localSize[2])),
	MY_MPI_FLOAT, _parameters.parallel.topNb, 204, PETSC_COMM_WORLD, &(requestP[6]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.bottomPressureReadBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 204, PETSC_COMM_WORLD, &(requestP[7]));

	// Send front and recv from back
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.frontPressureBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 205, PETSC_COMM_WORLD, &(requestP[8]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.backPressureReadBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.backNb, 205, PETSC_COMM_WORLD, &(requestP[9]));

	// Send back and recv from front
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.backPressureBuffer, ((localSize[0]) * (localSize[1])),
	MY_MPI_FLOAT, _parameters.parallel.backNb, 206, PETSC_COMM_WORLD,
			&(requestP[10]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.frontPressureReadBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 206, PETSC_COMM_WORLD, &(requestP[11]));

	for (int i = 0; i < 12; i++) {
		MPI_Wait(&(requestP[i]), &(statusP[i]));
	}

/*	if( rank == 1 ){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << readPStencil.leftPressureReadBuffer[i] <<"\n";
		}
	}

	fpp.close();*/
	pressurereadIterator.iterate();

}

void MessagePassingConfiguration::communicateViscosity(){

/*	int rank;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
	std::ofstream fpp;
	std::stringstream ss;
	ss << rank;
	std::string test = "commP";
	test+=ss.str();
	test+=".dat";
	fpp.open(  test.c_str());*/

	MPI_Request requestV[2 * 6];
	MPI_Status statusV[2 * 6];
	viscosityfillIterator.iterate();
	const int * localSize = fillVisStencil.localSize;

	// Now the actual comm to the left side process and recv from the right
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.leftViscosityBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 201, PETSC_COMM_WORLD, &(requestV[0]));
/*	if(rank == 1){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << fillPStencil.leftViscosityBuffer[i] <<"\n";
		}
	}*/
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.rightViscosityReadBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 201, PETSC_COMM_WORLD, &(requestV[1]) );

	// Send to right and recv from left
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.rightViscosityBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.rightNb, 202, PETSC_COMM_WORLD, &(requestV[2]));
/*	if(rank ==0 ){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << fillVisStencil.rightViscosityBuffer[i] <<"\n";
		}
	}*/
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.leftViscosityReadBuffer,
			((localSize[1]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.leftNb, 202, PETSC_COMM_WORLD, &(requestV[3]));

	// Send bottom and recv from top
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.bottomViscosityBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 203, PETSC_COMM_WORLD,&(requestV[4]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.topViscosityReadBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.topNb, 203, PETSC_COMM_WORLD, &(requestV[5]));

	// Send top and recv from bottom
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.topViscosityBuffer, ((localSize[0]) * (localSize[2])),
	MY_MPI_FLOAT, _parameters.parallel.topNb, 204, PETSC_COMM_WORLD,&(requestV[6]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.bottomViscosityReadBuffer,
			((localSize[0]) * (localSize[2])), MY_MPI_FLOAT,
			_parameters.parallel.bottomNb, 204, PETSC_COMM_WORLD, &(requestV[7]));

	// Send front and recv from back
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.frontViscosityBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 205, PETSC_COMM_WORLD,&(requestV[8]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.backViscosityReadBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.backNb, 205, PETSC_COMM_WORLD, &(requestV[9]));

	// Send back and recv from front
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillVisStencil.backViscosityBuffer, ((localSize[0]) * (localSize[1])),
	MY_MPI_FLOAT, _parameters.parallel.backNb, 206, PETSC_COMM_WORLD, &(requestV[10]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readVisStencil.frontViscosityReadBuffer,
			((localSize[0]) * (localSize[1])), MY_MPI_FLOAT,
			_parameters.parallel.frontNb, 206, PETSC_COMM_WORLD, &(requestV[11]));


/*	if( rank == 1 ){
		for (int i = 0; i < ((localSize[1]) * (localSize[2])); i++){
			fpp << readVisStencil.leftViscosityReadBuffer[i] <<"\n";
		}
	}

	fpp.close();*/
	for (int i = 0; i < 12; i++) {
		MPI_Wait(&(requestV[i]), &(statusV[i]));
	}
	viscosityreadIterator.iterate();

}
void MessagePassingConfiguration::communicateParameters(){

}
