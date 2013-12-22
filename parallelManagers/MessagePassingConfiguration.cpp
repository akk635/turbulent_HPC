/*
 * MessagePassingConfiguration.cpp
 *
 *  Created on: Dec 21, 2013
 *      Author: karthik
 */
# include "MessagePassingConfiguration.h"

MessagePassingConfiguration::MessagePassingConfiguration(
		Parameters & parameters, FlowField & flowfield) :
		_parameters(parameters), fillStencil(_parameters), readStencil(
				_parameters), _flowField(flowfield), velocityfillIterator(
				_flowField, _parameters, fillStencil, 1, -1), velocityreadIterator(
				_flowField, _parameters, readStencil, 1, -1), fillPStencil(
				_parameters), readPStencil(_parameters), pressurefillIterator(
				_flowField, _parameters, fillPStencil, 1, -1), pressurereadIterator(
				_flowField, _parameters, readPStencil, 1, -1) {
}

void MessagePassingConfiguration::communicateVelocity() {
	// Now filling the velocity buffers
	velocityfillIterator.iterate();
	MPI_Request request[2 * 6];
	MPI_Status status[2 * 6];
	const int * localSize = fillStencil.localSize;

	// Now the actual comm to the left side process and recv from the right
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.leftVelocityBuffer,
			((localSize[1]) * (localSize[2]))
					+ 2 * ((localSize[1] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.leftNb, 101, MPI_COMM_WORLD, &(request[0]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.rightVelocityReadBuffer,
			((localSize[1]) * (localSize[2]))
					+ 2 * ((localSize[1] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.rightNb, 101, MPI_COMM_WORLD, &(request[1]));

	// Send to right and recv from left
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.rightVelocityBuffer,
			((localSize[1]) * (localSize[2]))
					+ 2 * ((localSize[1] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.rightNb, 102, MPI_COMM_WORLD, &(request[2]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.leftVelocityReadBuffer,
			((localSize[1]) * (localSize[2]))
					+ 2 * ((localSize[1] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.leftNb, 102, MPI_COMM_WORLD, &(request[3]));

	// Send bottom and recv from top
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.bottomVelocityBuffer,
			((localSize[0]) * (localSize[2]))
					+ 2 * ((localSize[0] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.bottomNb, 103, MPI_COMM_WORLD, &(request[4]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.topVelocityReadBuffer,
			((localSize[0]) * (localSize[2]))
					+ 2 * ((localSize[0] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.topNb, 103, MPI_COMM_WORLD, &(request[5]));

	// Send top and recv from bottom
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.topVelocityBuffer,
			((localSize[0]) * (localSize[2]))
					+ 2 * ((localSize[0] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.topNb, 104, MPI_COMM_WORLD, &(request[6]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.bottomVelocityReadBuffer,
			((localSize[0]) * (localSize[2]))
					+ 2 * ((localSize[0] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.bottomNb, 104, MPI_COMM_WORLD, &(request[7]));

	// Send front and recv from back
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.frontVelocityBuffer,
			((localSize[0]) * (localSize[0]))
					+ 2 * ((localSize[0] + 1) * (localSize[1] + 1)), MPI_DOUBLE,
			_parameters.parallel.frontNb, 105, MPI_COMM_WORLD, &(request[8]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.backVelocityReadBuffer,
			((localSize[0]) * (localSize[1]))
					+ 2 * ((localSize[0] + 1) * (localSize[1] + 1)), MPI_DOUBLE,
			_parameters.parallel.backNb, 105, MPI_COMM_WORLD, &(request[9]));

	// Send back and recv from front
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.backVelocityBuffer,
			((localSize[0]) * (localSize[1]))
					+ 2 * ((localSize[0] + 1) * (localSize[1] + 1)), MPI_DOUBLE,
			_parameters.parallel.backNb, 106, MPI_COMM_WORLD, &(request[10]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.frontVelocityReadBuffer,
			((localSize[0]) * (localSize[1]))
					+ 2 * ((localSize[0] + 1) * (localSize[1] + 1)), MPI_DOUBLE,
			_parameters.parallel.frontNb, 106, MPI_COMM_WORLD, &(request[11]));

	for (int i = 0; i < 12; i++) {
		MPI_Wait(&(request[i]), &(status[i]));
	}

	// Writing the buffers into appropriate flowfields
	velocityreadIterator.iterate();
}

void MessagePassingConfiguration::communicatePressure() {
	pressurefillIterator.iterate();
	MPI_Request request[2 * 6];
	MPI_Status status[2 * 6];
	const int * localSize = fillPStencil.localSize;

	// Now the actual comm to the left side process and recv from the right
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.leftPressureBuffer,
			((localSize[1]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.leftNb, 201, MPI_COMM_WORLD, &(request[0]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.rightPressureReadBuffer,
			((localSize[1]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.rightNb, 201, MPI_COMM_WORLD, &(request[1]));

	// Send to right and recv from left
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.rightPressureBuffer,
			((localSize[1]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.rightNb, 202, MPI_COMM_WORLD, &(request[2]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readPStencil.leftPressureReadBuffer,
			((localSize[1]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.leftNb, 202, MPI_COMM_WORLD, &(request[3]));

	// Send bottom and recv from top
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillPStencil.bottomPressureBuffer,
			((localSize[0]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.bottomNb, 203, MPI_COMM_WORLD, &(request[4]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.topVelocityReadBuffer,
			((localSize[0]) * (localSize[2]))
					+ 2 * ((localSize[0] + 1) * (localSize[2] + 1)), MPI_DOUBLE,
			_parameters.parallel.topNb, 203, MPI_COMM_WORLD, &(request[5]));

	// Send top and recv from bottom
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.topVelocityBuffer, ((localSize[0]) * (localSize[2])),
	MPI_DOUBLE, _parameters.parallel.topNb, 204, MPI_COMM_WORLD, &(request[6]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.bottomVelocityReadBuffer,
			((localSize[0]) * (localSize[2])), MPI_DOUBLE,
			_parameters.parallel.bottomNb, 204, MPI_COMM_WORLD, &(request[7]));

	// Send front and recv from back
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.frontVelocityBuffer,
			((localSize[0]) * (localSize[1])), MPI_DOUBLE,
			_parameters.parallel.frontNb, 205, MPI_COMM_WORLD, &(request[8]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.backVelocityReadBuffer,
			((localSize[0]) * (localSize[1])), MPI_DOUBLE,
			_parameters.parallel.backNb, 205, MPI_COMM_WORLD, &(request[9]));

	// Send back and recv from front
	// MPI_Isend(buffer,count,type,dest,tag,comm,request)
	MPI_Isend(fillStencil.backVelocityBuffer, ((localSize[0]) * (localSize[1])),
	MPI_DOUBLE, _parameters.parallel.backNb, 206, MPI_COMM_WORLD,
			&(request[10]));
	// MPI_Irecv(buffer,count,type,source,tag,comm,request)
	MPI_Irecv(readStencil.frontVelocityReadBuffer,
			((localSize[0]) * (localSize[1])), MPI_DOUBLE,
			_parameters.parallel.frontNb, 206, MPI_COMM_WORLD, &(request[11]));

	for (int i = 0; i < 12; i++) {
		MPI_Wait(&(request[i]), &(status[i]));
	}

}
