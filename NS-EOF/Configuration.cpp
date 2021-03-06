#include "Configuration.h"
#include "3rdparty/tinyxml2/tinyxml2.h"
#include <string>

void readFloatMandatory(FLOAT & storage, tinyxml2::XMLElement *node, const char* tag){
    double value;   // Use to be able to select precision
    if (node->QueryDoubleAttribute(tag, &value) != tinyxml2::XML_NO_ERROR){
        handleError(1, "Error while reading mandatory argument");
    } else {
        storage = (FLOAT)value;
    }
}

void readFloatOptional(FLOAT & storage, tinyxml2::XMLElement *node, const char* tag,
                       FLOAT defaultValue = 0){
    double value;   // Use to be able to select precision
    int result = node->QueryDoubleAttribute(tag, &value);
    if (result == tinyxml2::XML_NO_ATTRIBUTE){
        storage = defaultValue;
    } else if (result == tinyxml2::XML_WRONG_ATTRIBUTE_TYPE){
        handleError(1, "Error while reading optional argument");
    } else {
        storage = (FLOAT)value;
    }
}

void readIntMandatory(int & storage, tinyxml2::XMLElement *node, const char* tag){
    int value;
    if (node->QueryIntAttribute(tag, &value) != tinyxml2::XML_NO_ERROR){
        handleError(1, "Error while reading mandatory argument");
    } else {
        storage = value;
    }
}

void readIntOptional(int & storage, tinyxml2::XMLElement *node, const char* tag,
                       int defaultValue = 0){
    int result = node->QueryIntAttribute(tag, &storage);
    if (result == tinyxml2::XML_NO_ATTRIBUTE){
        storage = defaultValue;
    } else if (result == tinyxml2::XML_WRONG_ATTRIBUTE_TYPE){
        handleError(1, "Error while reading optional argument");
    }
}

void readBoolOptional(bool & storage, tinyxml2::XMLElement *node, const char* tag,
                      bool defaultValue = false){
    int result = node->QueryBoolAttribute(tag, &storage);
    if (result == tinyxml2::XML_NO_ATTRIBUTE){
        storage = defaultValue;
    } else if (result == tinyxml2::XML_WRONG_ATTRIBUTE_TYPE){
        handleError(1, "Error while reading optional argument");
    }
}

void readStringMandatory(std::string & storage, tinyxml2::XMLElement *node){
    const char *myText = node->GetText();
    if (myText == NULL){
        const std::string nodename = node->Name();
        std::cerr << "ERROR in file " << __FILE__ << ", line " << __LINE__ << ": ";
        std::cerr << "No string specified for this node: " << nodename << std::endl;
        exit(2);
    } else {
      storage = node->GetText();
      if (!storage.compare("")){
        handleError(1, "Missing mandatory string!");
      }
    }
}

void readWall(tinyxml2::XMLElement *wall, FLOAT *vector, FLOAT &scalar){
    tinyxml2::XMLElement *quantity = wall->FirstChildElement("vector");
    if (quantity != NULL){
        readFloatOptional(vector[0], quantity, "x");
        readFloatOptional(vector[1], quantity, "y");
        readFloatOptional(vector[2], quantity, "z");
    }
    quantity = wall->FirstChildElement("scalar");
    if (quantity != NULL){
        readFloatOptional(scalar, quantity, "value");
    }
}

void broadcastString (std::string & target, const MPI_Comm & communicator, int root = 0){
    int stringSize, rank;
    MPI_Comm_rank(communicator, &rank);
    if (rank == root){
        stringSize = target.size();
    }
    MPI_Bcast(&stringSize, 1, MPI_INT, 0, communicator);
    char *name = new char[stringSize + 1];    // One more for the null character
    if (rank == root) {
        target.copy(name, stringSize, 0);
    }
    name[stringSize] = '\0';
    MPI_Bcast(name, stringSize+1, MPI_CHAR, 0, communicator);
    if (rank != root){
        target = name;
    }
    delete [] name; name = NULL;
}

Configuration::Configuration(){
    _filename = "";
}

Configuration::Configuration(const std::string & filename){
    _filename = filename;
}

void Configuration::setFileName(const std::string & filename){
    _filename = filename;
}

void Configuration::loadParameters(Parameters & parameters, const MPI_Comm & communicator){
    tinyxml2::XMLDocument confFile;
    tinyxml2::XMLElement *node;
    tinyxml2::XMLElement *subNode;

    int rank;

    MPI_Comm_rank(communicator, &rank);

    // we only read on rank 0; afterwards, all configuration parameters are broadcasted to all processes.
    // So, if you add new parameters in the configuration, make sure to broadcast them to the other processes!
    if (rank == 0){

        // Parse the configuration file and check validity
        confFile.LoadFile(_filename.c_str());
        if (confFile.FirstChildElement() == NULL){
            handleError(1, "Error parsing the configuration file");
        }

        //--------------------------------------------------
        // Load geometric parameters
        //--------------------------------------------------
        node = confFile.FirstChildElement()->FirstChildElement("geometry");

        if (node == NULL){
            handleError(1, "Error loading geometry properties");
        }

        readIntMandatory(parameters.geometry.sizeX, node, "sizeX");
        readIntMandatory(parameters.geometry.sizeY, node, "sizeY");
        readIntOptional (parameters.geometry.sizeZ, node, "sizeZ");

        if (parameters.geometry.sizeX < 2 || parameters.geometry.sizeY < 2 ||
                parameters.geometry.sizeZ < 0){
            handleError(1, "Invalid size specified in configuration file");
        }

        parameters.geometry.dim = 0;
        if (node->QueryIntAttribute("dim", &(parameters.geometry.dim)) !=
                tinyxml2::XML_WRONG_ATTRIBUTE_TYPE){
            if (parameters.geometry.dim == 0){
                if (parameters.geometry.sizeZ == 0){
                    parameters.geometry.sizeZ = 1;
                    parameters.geometry.dim = 2;
                } else {
                    parameters.geometry.dim = 3;
                }
            }
        }

        if (parameters.geometry.dim == 3 && parameters.geometry.sizeZ == 1){
            handleError(1, "Inconsistent data: 3D geometry specified with Z size zero");
        }

        // Determine the sizes of the cells
        readFloatOptional (parameters.geometry.dx, node, "dx");
        readFloatOptional (parameters.geometry.dy, node, "dy");
        readFloatOptional (parameters.geometry.dz, node, "dz");

        //std::cout << "dx " << parameters.geometry.dx << std::endl;

        FLOAT lengthX = 0,
              lengthY = 0,
              lengthZ = 0;

        readFloatOptional (lengthX, node, "lengthX");
        readFloatOptional (lengthY, node, "lengthY");
        readFloatOptional (lengthZ, node, "lengthZ");


        // Do not define both overall length and cell length
        if ((parameters.geometry.dx && lengthX) || !(parameters.geometry.dx || lengthX)){
            handleError(1,"Invalid definition for size. Define either length of element or length of domain for dimension X");
        }
        if ((parameters.geometry.dy && lengthY) || !(parameters.geometry.dy || lengthY)){
            handleError(1,"Invalid definition for size. Define either length of element or length of domain for dimension Y");
        }
        if ((parameters.geometry.dz && lengthZ) || !(parameters.geometry.dz || lengthZ)){
            handleError(1,"Invalid definition for size. Define either length of element or length of domain for dimension Z");
        }

        if (lengthX){
            parameters.geometry.dx = lengthX / parameters.geometry.sizeX;
        }
        if (lengthY){
            parameters.geometry.dy = lengthY / parameters.geometry.sizeY;
        }
        if (lengthZ){
            parameters.geometry.dz = lengthZ / parameters.geometry.sizeZ;
        }
        // Now, the size of the elements should be set

        if (parameters.geometry.dx <= 0 || parameters.geometry.dy <= 0){
            handleError(1, "Invalid specification for dx or dy in the configuration file");
        }

        if (parameters.geometry.dz <= 0 && parameters.geometry.dim == 3){
            handleError(1, "Invalid specification for dz in the configuration file");
        }

        _dim = parameters.geometry.dim;

        //--------------------------------------------------
        // Timestep parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("timestep");

        if (node == NULL){
            handleError(1, "Error loading timestep parameters");
        }

        readFloatOptional(parameters.timestep.dt, node, "dt", 1);
        readFloatOptional(parameters.timestep.tau, node, "tau", 0.5);

        //--------------------------------------------------
        // Flow parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("flow");

        if (node == NULL){
            handleError(1, "Error loading flow parameters");
        }

        readFloatMandatory(parameters.flow.Re, node, "Re");

        //--------------------------------------------------
        // Solver parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("solver");

        if (node == NULL){
            handleError(1, "Error loading solver parameters");
        }

        readFloatMandatory(parameters.solver.gamma, node, "gamma");
        readIntOptional (parameters.solver.maxIterations, node, "maxIterations");

        //--------------------------------------------------
        // Environmental parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("environment");

        if (node == NULL){
            handleError(1, "Error loading environmental  parameters");
        }

        readFloatOptional(parameters.environment.gx, node, "gx");
        readFloatOptional(parameters.environment.gy, node, "gy");
        readFloatOptional(parameters.environment.gz, node, "gz");

        //--------------------------------------------------
        // Simulation parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("simulation");

        if (node == NULL){
            handleError(1, "Error loading simulation parameters");
        }

        readFloatMandatory(parameters.simulation.finalTime, node, "finalTime");

        subNode = node->FirstChildElement("scenario");
        if (subNode != NULL){
            readStringMandatory(parameters.simulation.scenario, subNode);
        } else {
            handleError (1, "Missing scenario in simulation parameters");
        }

        //--------------------------------------------------
        // Simulation parameters
        //--------------------------------------------------

                node = confFile.FirstChildElement()->FirstChildElement("turbulent");

                if (node == NULL){
                    handleError(1, "Error loading turbulent parameters");
                }

                readFloatMandatory(parameters.turbulent.kappa, node, "kappa");

                subNode = node->FirstChildElement("scenario");
                if (subNode != NULL){
                    readStringMandatory(parameters.turbulent.turbulent_scenario, subNode);
                } else {
                    handleError (1, "Missing scenario in turbulent parameters");
                }

        //--------------------------------------------------
        // VTK parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("vtk");

        if (node == NULL){
            handleError(1, "Error loading VTK parameters");
        }

        readFloatOptional(parameters.vtk.interval, node, "interval");
        readStringMandatory(parameters.vtk.prefix, node);

        //--------------------------------------------------
        // StdOut parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("stdOut");

        if (node == NULL){
            handleError(1, "Error loading StdOut parameters");
        }

        // If no value given, print every step
        readFloatOptional(parameters.stdOut.interval, node, "interval", 1);

        //--------------------------------------------------
        // Parallel parameters
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("parallel");

        if (node == NULL){
            handleError(1, "Error loading parallel parameters");
        }

        readIntOptional(parameters.parallel.numProcessors[0], node, "numProcessorsX", 1);
        readIntOptional(parameters.parallel.numProcessors[1], node, "numProcessorsY", 1);
        readIntOptional(parameters.parallel.numProcessors[2], node, "numProcessorsZ", 1);

        // Start neighbors on null in case that no parallel configuration is used later.
        parameters.parallel.leftNb = MPI_PROC_NULL;
        parameters.parallel.rightNb = MPI_PROC_NULL;
        parameters.parallel.bottomNb = MPI_PROC_NULL;
        parameters.parallel.topNb = MPI_PROC_NULL;
        parameters.parallel.frontNb = MPI_PROC_NULL;
        parameters.parallel.backNb = MPI_PROC_NULL;

        // Yet more parameters initialized in case that no parallel configuration is applied
        parameters.parallel.localSize[0] = parameters.geometry.sizeX;
        parameters.parallel.localSize[1] = parameters.geometry.sizeY;
        parameters.parallel.localSize[2] = parameters.geometry.sizeZ;

        parameters.parallel.firstCorner[0] = 0;
        parameters.parallel.firstCorner[1] = 0;
        parameters.parallel.firstCorner[2] = 0;

        // VTK output is named after the rank, so we define it here, again, in case that it's not
        // initialized anywhere else.
        parameters.parallel.rank = rank;

        //--------------------------------------------------
        // Walls
        //--------------------------------------------------

        node = confFile.FirstChildElement()->FirstChildElement("walls");

        if (node == NULL){
            handleError(1, "Error loading wall parameters");
        }


        tinyxml2::XMLElement *wall;
        wall = node->FirstChildElement("left");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorLeft, parameters.walls.scalarLeft);
        }

        wall = node->FirstChildElement("right");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorRight, parameters.walls.scalarRight);
        }

        wall = node->FirstChildElement("bottom");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorBottom, parameters.walls.scalarBottom);
        }

        wall = node->FirstChildElement("top");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorTop, parameters.walls.scalarTop);
        }

        wall = node->FirstChildElement("front");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorFront, parameters.walls.scalarFront);
        }

        wall = node->FirstChildElement("back");
        if (wall != NULL){
            readWall(wall, parameters.walls.vectorBack, parameters.walls.scalarBack);
        }

        // Set the scalar values to zero
        parameters.walls.scalarLeft   = 0.0;
        parameters.walls.scalarRight  = 0.0;
        parameters.walls.scalarBottom = 0.0;
        parameters.walls.scalarTop    = 0.0;
        parameters.walls.scalarFront  = 0.0;
        parameters.walls.scalarBack   = 0.0;


        //--------------------------------------------------
        // Backward facing step
        //--------------------------------------------------
        parameters.bfStep.xRatio = -1.0;
        parameters.bfStep.yRatio = -1.0;
        node = confFile.FirstChildElement()->FirstChildElement("backwardFacingStep");
        if (node != NULL){
            readFloatMandatory(parameters.bfStep.xRatio, node, "xRatio");
            readFloatMandatory(parameters.bfStep.yRatio, node, "yRatio");
        }
        parameters.bfStep.width = (int)(parameters.geometry.sizeX * parameters.bfStep.xRatio);
        parameters.bfStep.height = (int)(parameters.geometry.sizeY * parameters.bfStep.yRatio);
    }

    // Broadcasting of the values
    // Golbal size of the domain
    MPI_Bcast(&(parameters.geometry.sizeX), 1, MPI_INT, 0, communicator);
    MPI_Bcast(&(parameters.geometry.sizeY), 1, MPI_INT, 0, communicator);
    MPI_Bcast(&(parameters.geometry.sizeZ), 1, MPI_INT, 0, communicator);

    MPI_Bcast(&(parameters.geometry.dim), 1, MPI_INT, 0, communicator);

    MPI_Bcast(&(parameters.geometry.dx), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.geometry.dy), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.geometry.dz), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.timestep.dt),  1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.timestep.tau), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.flow.Re), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.solver.gamma),         1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.solver.maxIterations), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.environment.gx), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.environment.gy), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.environment.gz), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.simulation.finalTime), 1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.vtk.interval), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.stdOut.interval), 1, MPI_INT, 0, communicator);

    broadcastString (parameters.vtk.prefix, communicator);
    broadcastString (parameters.simulation.scenario, communicator);

    MPI_Bcast(&(parameters.bfStep.xRatio), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.bfStep.yRatio), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.bfStep.width), 1, MPI_INT, 0, communicator);
    MPI_Bcast(&(parameters.bfStep.height), 1, MPI_INT, 0, communicator);

    MPI_Bcast(parameters.parallel.numProcessors, 3, MPI_INT, 0, communicator);

    MPI_Bcast(&(parameters.walls.scalarLeft),   1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.walls.scalarRight),  1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.walls.scalarBottom), 1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.walls.scalarTop),    1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.walls.scalarFront),  1, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(&(parameters.walls.scalarBack),   1, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(parameters.walls.vectorLeft,   3, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(parameters.walls.vectorRight,  3, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(parameters.walls.vectorBottom, 3, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(parameters.walls.vectorTop,    3, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(parameters.walls.vectorFront,  3, MY_MPI_FLOAT, 0, communicator);
    MPI_Bcast(parameters.walls.vectorBack,   3, MY_MPI_FLOAT, 0, communicator);

    MPI_Bcast(&(parameters.turbulent.kappa), 1, MY_MPI_FLOAT, 0, communicator);
    broadcastString (parameters.turbulent.turbulent_scenario, communicator);

}
