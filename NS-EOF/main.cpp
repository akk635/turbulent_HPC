#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Configuration.h"
#include "Simulation.h"
#include "parallelManagers/PetscParallelConfiguration.h"
#include "SimpleTimer.h"

int main( int argc, char *argv[] ) {

    // Parallelization related. Initialize and identify
    // ---------------------------------------------------
    int rank;   // This processor's identifier
    int nproc;  // Number of processors in the group
    // MPI_Init ( &argc, &argv );
    PetscInitialize( &argc, &argv, PETSC_NULL, PETSC_NULL );
    MPI_Comm_size( PETSC_COMM_WORLD, &nproc );
    MPI_Comm_rank( PETSC_COMM_WORLD, &rank );
    std::cout << "Rank: " << rank << ", Nproc: " << nproc << std::endl;
    //----------------------------------------------------

    if ( argc < 3 ) {
        handleError(
                1,
                "A configuration file is required! Call the program by ./main configfile.xml validatelength" );
    }

    // read configuration and store information in parameters object
    Configuration configuration( argv[1] );
    Parameters parameters;
    configuration.loadParameters( parameters );
    PetscParallelConfiguration parallelConfiguration( parameters );
    FlowField *flowField = NULL;
    Simulation *simulation = NULL;

    std::cout << " Processor " << parameters.parallel.rank << " with index ";
    std::cout << parameters.parallel.indices[0] << ",";
    std::cout << parameters.parallel.indices[1] << ",";
    std::cout << parameters.parallel.indices[2];
    std::cout << " is computing the size of its subdomain and obtains ";
    std::cout << parameters.parallel.localSize[0] << ", ";
    std::cout << parameters.parallel.localSize[1] << " and ";
    std::cout << parameters.parallel.localSize[2];
    std::cout << ". top neighbour: " << parameters.parallel.topNb;
    std::cout << ", bottom neighbour: " << parameters.parallel.bottomNb;
    std::cout << std::endl;

    // initialise simulation
    std::cout << "Start DNS simulation in " << parameters.geometry.dim << "D" << std::endl;
    flowField = new FlowField( parameters );
    if ( flowField == NULL ) {
        handleError( 1, "flowField==NULL!" );
    }
    simulation = new Simulation( parameters, *flowField );
    if ( simulation == NULL ) {
        handleError( 1, "simulation==NULL!" );
    }

    // TODO WORKSHEET 1: plot initial state
    if ( parameters.vtk.prefix == "taylorgreen_sequential_result" ) {
        simulation->initializeVelocity();
    }
    if ( parameters.vtk.prefix == "channel_result" ) {
        simulation->initFlagField();
    }
    simulation->initializeNearestWall();
    simulation->initializeBoundLayerThickness();

    // TODO WORKSHEET 2: loop over time and
    //                   - solve one time step
    //                   - plot VTK output (if required at this time step)
    //                   - write simulation status to terminal (if required at this time step)
    MPI_Barrier (PETSC_COMM_WORLD);

    SimpleTimer timer;

    if ( rank == 0 ) {
        std::cout << "parameters.vtk.interval = " << parameters.vtk.interval << std::endl;
    }
    parameters.vtk.vtkCounter = 0;
    parameters.simulation.currentTime = 0;
    simulation->plotVTK( parameters.vtk.vtkCounter, rank );
    if ( rank == 0 ) {
        std::cout << "parameters.vtk.vtkCounter = " << parameters.vtk.vtkCounter << std::endl;
        std::cout << "parameters.simulation.currentTime = " << parameters.simulation.currentTime
                  << std::endl;
    }

    /*    for ( int i = 0; i < 5; i++ ) {
     simulation->solveTimestep();
     std::cout << parameters.timestep.dt <<std::endl;
     ( parameters.vtk.vtkCounter )++;
     if ( i < 10 )
     simulation->plotVTK( parameters.vtk.vtkCounter, rank );
     }

     PetscFinalize();
     return 0;*/

    // argv[2] gives u the length in mm/100 scale
    int x_pos = atoi( argv[2] );

    std::cout << x_pos << std::endl;

    while ( parameters.simulation.currentTime <= parameters.simulation.finalTime ) {
        simulation->solveTimestep();
        std::cout << parameters.timestep.dt << std::endl;
        parameters.simulation.currentTime += parameters.timestep.dt;
        if ( parameters.simulation.currentTime
                > ( parameters.vtk.vtkCounter + 1 ) * parameters.vtk.interval - 0.001 ) {
            ( parameters.vtk.vtkCounter )++;
            simulation->plotVTK( parameters.vtk.vtkCounter, rank );
            if ( rank == 0 ) {
                std::cout << "parameters.vtk.vtkCounter = " << parameters.vtk.vtkCounter
                          << std::endl;
                std::cout << "parameters.simulation.currentTime = "
                          << parameters.simulation.currentTime << std::endl;
            }
        }
    }

    MPI_Barrier( PETSC_COMM_WORLD );

    /*    FLOAT timeElapsed = timer.getTimeAndRestart();
     std::ofstream fpres;
     std::string result = "result_solver.csv";
     fpres.open( result.c_str(), std::ios::app );
     fpres.seekp( 0, std::ios_base::end );
     fpres << argv[3] << ',' << argv[5] << ',' << argv[7] << "\n";
     fpres << timeElapsed << "\n";
     fpres.close();*/

    // Explicit pos in global domain
    x_pos = x_pos / parameters.geometry.dx;
    int z_pos = parameters.geometry.sizeZ / 2;
    if ( x_pos >= parameters.parallel.firstCorner[0]
            && x_pos <= ( parameters.parallel.firstCorner[0] + parameters.parallel.localSize[0] )
            && z_pos >= parameters.parallel.firstCorner[2]
            && z_pos <= ( parameters.parallel.firstCorner[2] + parameters.parallel.localSize[2] ) ) {
        // For making the position local
        x_pos -= parameters.parallel.firstCorner[0];
        z_pos -= parameters.parallel.firstCorner[2];
        std::cout << x_pos << std::endl;
        std::cout << z_pos << std::endl;
        FLOAT velocity_magn = 0;
        std::ofstream fvalidation;
        std::string validation = "validation";
        std::stringstream ss;
        ss << rank;
        validation += ss.str();
        validation += ".csv";
        fvalidation.open( validation.c_str(), std::ios::app );
        fvalidation << "y position , " << "Velocities" << std::endl;
        fvalidation.seekp( 0, std::ios_base::end );
        for ( int j = 0; j < parameters.parallel.localSize[1]; j++ ) {
            velocity_magn =
                    sqrt( ( flowField->getVelocity().getVector(
                            x_pos+2, j + 2, z_pos + 2 )[0]
                            * flowField->getVelocity().getVector(
                                    x_pos + 2, j + 2, z_pos + 2 )[0] )
                            + ( flowField->getVelocity().getVector(
                                    x_pos + 2, j + 2, z_pos + 2 )[1]
                                    * flowField->getVelocity().getVector(
                                            x_pos + 2, j + 2, z_pos + 2 )[1] )
                            + ( flowField->getVelocity().getVector(
                                    x_pos + 2, j + 2, z_pos + 2 )[2]
                                    * flowField->getVelocity().getVector(
                                            x_pos + 2, j + 2, z_pos + 2 )[2] ) );

            fvalidation << ( j + parameters.parallel.firstCorner[1] + 0.5 ) * parameters.geometry.dy
                        << ',' << velocity_magn << std::endl;
        }
        fvalidation.close();
    }

    /*	if (rank == 0) {
     FLOAT timeElapsed = timer.getTimeAndRestart();
     std::ofstream fpres;
     std::string result = "result.csv";
     fpres.open(result.c_str(), std::ios::app);
     fpres.seekp(0, std::ios_base::end);
     if( nproc == 1)
     fpres <<"Procs , " << "TimeElapsed" << std::endl;
     fpres << nproc << ',' << timeElapsed << "\n";
     fpres.close();
     }*/

    delete simulation;
    simulation = NULL;
    delete flowField;
    flowField = NULL;
    PetscFinalize();
}
