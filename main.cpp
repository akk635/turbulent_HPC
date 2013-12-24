#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Configuration.h"
#include "Simulation.h"
#include "parallelManagers/PetscParallelConfiguration.h"

int main (int argc, char *argv[]) {

    // Parallelization related. Initialize and identify
    // ---------------------------------------------------
    int rank;   // This processor's identifier
    int nproc;  // Number of processors in the group
    // MPI_Init ( &argc, &argv );
    PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
    MPI_Comm_size(PETSC_COMM_WORLD, &nproc);
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    std::cout << "Rank: " << rank << ", Nproc: " << nproc << std::endl;
    //----------------------------------------------------

    if (argc != 2){
        handleError(1, "A configuration file is required! Call the program by ./main configfile.xml");
    }

    // read configuration and store information in parameters object
    Configuration configuration(argv[1]);
    Parameters parameters;
    configuration.loadParameters(parameters);
    PetscParallelConfiguration parallelConfiguration(parameters);
    FlowField *flowField = NULL;
    Simulation *simulation = NULL;

    std::cout << "Processor " << parameters.parallel.rank << " with index ";
    std::cout << parameters.parallel.indices[0] << ",";
    std::cout << parameters.parallel.indices[1] << ",";
    std::cout << parameters.parallel.indices[2];
    std::cout <<    " is computing the size of its subdomain and obtains ";
    std::cout << parameters.parallel.localSize[0] << ", ";
    std::cout << parameters.parallel.localSize[1] << " and ";
    std::cout << parameters.parallel.localSize[2];
    std::cout << ". Left neighbour: " << parameters.parallel.leftNb;
    std::cout << ", right neighbour: " << parameters.parallel.rightNb;
    std::cout << std::endl;

    // initialise simulation
    std::cout << "Start DNS simulation in " << parameters.geometry.dim << "D" << std::endl;
    flowField = new FlowField(parameters);
    if(flowField == NULL){ handleError(1, "flowField==NULL!"); }
    simulation = new Simulation(parameters,*flowField);
    if(simulation == NULL){ handleError(1, "simulation==NULL!"); }

    // TODO WORKSHEET 1: plot initial state
    if( parameters.vtk.prefix == "taylorgreen_sequential_result" ) {
        simulation->initializeVelocity();
    }
    if( parameters.vtk.prefix == "channel_result" ){
        simulation->initFlagField();
    }
    // Initialization state
    simulation->plotVTK(0, rank);
//    simulation->solveTimestep();

/*    std::cout << "parameters.timestep.dt = " << parameters.timestep.dt << std::endl;
    simulation->plotVTK(1, rank);*/
    // TODO WORKSHEET 2: loop over time and
    //                   - solve one time step
    //                   - plot VTK output (if required at this time step)
    //                   - write simulation status to terminal (if required at this time step)

    parameters.vtk.vtkCounter=0;
    parameters.simulation.currentTime=0;
    while (parameters.simulation.currentTime <= parameters.simulation.finalTime){
        simulation->solveTimestep();
        parameters.simulation.currentTime+=parameters.timestep.dt;
        if ( parameters.simulation.currentTime > (parameters.vtk.vtkCounter + 1) * parameters.vtk.interval - 0.00001){
            (parameters.vtk.vtkCounter)++;
            simulation->plotVTK(parameters.vtk.vtkCounter,rank);
            std::cout << "parameters.vtk.vtkCounter = " << parameters.vtk.vtkCounter << std::endl;
            std::cout << "parameters.simulation.currentTime = " << parameters.simulation.currentTime << std::endl;
        }
    }

    // TODO WORKSHEET 2: plot final VTK output

    delete simulation; simulation=NULL;
    delete flowField;  flowField= NULL;
    PetscFinalize();
}
