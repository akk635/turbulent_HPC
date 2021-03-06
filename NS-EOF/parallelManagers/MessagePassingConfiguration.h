/*
 * MessagePassingConfiguration.h
 *
 *  Created on: Dec 21, 2013
 *      Author: karthik
 */

#ifndef MESSAGEPASSINGCONFIGURATION_H_
#define MESSAGEPASSINGCONFIGURATION_H_

#include "../Parameters.h"
#include "../Definitions.h"
#include "../stencils/VelocityBufferFillStencil.h"
#include "../stencils/VelocityBufferReadStencil.h"
#include "../stencils/PressureBufferFillStencil.h"
#include "../stencils/PressureBufferReadStencil.h"
#include "../stencils/ViscosityBufferFillStencil.h"
#include "../stencils/ViscosityBufferReadStencil.h"
#include "../Iterators.h"
#include "../FlowField.h"

// Class for the message passing of the pressures and velocities
class MessagePassingConfiguration{
private:
    Parameters & _parameters;   //! Reference to the parameters
    FlowField & _flowField;

    // To read from the processors
    VelocityBufferFillStencil fillStencil;
    VelocityBufferReadStencil readStencil;
    ParallelBoundaryIterator<FlowField> velocityfillIterator;
    ParallelBoundaryIterator<FlowField> velocityreadIterator;

    PressureBufferFillStencil fillPStencil;
    PressureBufferReadStencil readPStencil;
    ParallelBoundaryIterator<FlowField> pressurefillIterator;
    ParallelBoundaryIterator<FlowField> pressurereadIterator;

    ViscosityBufferFillStencil fillVisStencil;
    ViscosityBufferReadStencil readVisStencil;
    ParallelBoundaryIterator<FlowField> viscosityfillIterator;
    ParallelBoundaryIterator<FlowField> viscosityreadIterator;

public:
    MessagePassingConfiguration(Parameters & parameters, FlowField & flowfield);
    void communicateVelocity();
    void communicatePressure();
    void communicateViscosity();
    void communicateParameters();
    /** Destructor */
    ~MessagePassingConfiguration(){};
};




#endif /* MESSAGEPASSINGCONFIGURATION_H_ */
