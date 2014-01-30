/*
 * InitNearestWallStencil.h
 *
 *  Created on: Jan 14, 2014
 *      Author: Valizadeh
 */

#ifndef INITNEARESTWALLSTENCIL_H_
#define INITNEARESTWALLSTENCIL_H_

#include "../Parameters.h"
#include "../DataStructures.h"
#include "../FlowField.h"
#include "../Stencil.h"
#include <math.h>

class InitNearestWallStencil : public FieldStencil<FlowField> {
 private:
    const int xLimit;   //! Cells up to and including this will be set as obstacles
    const int yLimit;   //! Same as for x
 public:
    InitNearestWallStencil( const Parameters & parameters )
            : FieldStencil<FlowField>( parameters ),
              xLimit( parameters.bfStep.width ),
              yLimit( parameters.bfStep.height ) {
    }

    //		~InitNearestWallStencil(){
    //		}

    // For initialising the 2d case
    void apply( FlowField & flowField, int i, int j ) {

    }

    // For initialising the 3d case
    void apply( FlowField & flowField, int i, int j, int k ) {

        const int obstacle = flowField.getFlags().getValue( i, j, k );
        if ( ( obstacle & OBSTACLE_SELF ) == 0 ) {
            FLOAT distance = ( _parameters.geometry.sizeY
                    - ( _parameters.parallel.firstCorner[1] + ( j - 2 + 0.5 ) ) )
                    * _parameters.geometry.dy;
            FLOAT temp;
            if ( ( temp = ( _parameters.parallel.firstCorner[1] + ( j - 2 + 0.5 ) )
                    * _parameters.geometry.dy ) < distance ) {
                distance = temp;
            }
            if ( ( temp = ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) )
                    * _parameters.geometry.dz ) < distance ) {
                distance = temp;
            }
            if ( ( temp = ( _parameters.geometry.sizeZ
                    - ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) ) )
                    * _parameters.geometry.dz ) < distance ) {
                distance = temp;
            }

            if ( _parameters.simulation.scenario != "channel" ) {
                if ( ( temp = ( _parameters.geometry.sizeX
                        - ( _parameters.parallel.firstCorner[0] + ( i - 2 + 0.5 ) ) )
                        * _parameters.geometry.dx ) < distance ) {
                    distance = temp;
                }
                if ( ( temp = ( _parameters.parallel.firstCorner[0] + ( i - 2 + 0.5 ) )
                        * _parameters.geometry.dx ) < distance ) {
                    distance = temp;
                }
            }

            flowField.getDistanceToNearWall().getScalar( i, j, k ) = distance;
        }

        // assuming for nprocs in X dirction only considering X offset in 1 proc is sufficient

        const int *org = _parameters.parallel.firstCorner;
        // SPecial region if there is a step
        if(flowField.getFlags().getValue(org[0] + 2,org[1] + 2,org[2]+2) == OBSTACLE_SELF){
            int wallOffset[3];
            // First special region
            if (i + _parameters.parallel.firstCorner[0] - 2 < xLimit ){
                wallOffset[1] = _parameters.parallel.firstCorner[1] + yLimit;
                FLOAT distance = ( _parameters.geometry.sizeY
                        - ( wallOffset[1] + ( j - 2 + 0.5 ) ) )
                        * _parameters.geometry.dy;
                FLOAT temp;
                if ( ( temp = ( _parameters.parallel.firstCorner[1] + ( j - 2 + 0.5 ) )
                        * wallOffset[1] ) < distance ) {
                    distance = temp;
                }
                if ( ( temp = ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) )
                        * _parameters.geometry.dz ) < distance ) {
                    distance = temp;
                }
                if ( ( temp = ( _parameters.geometry.sizeZ
                        - ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) ) )
                        * _parameters.geometry.dz ) < distance ) {
                    distance = temp;
                }

                if ( _parameters.simulation.scenario != "channel" ) {
                    if ( ( temp = ( _parameters.geometry.sizeX
                            - ( _parameters.parallel.firstCorner[0] + ( i - 2 + 0.5 ) ) )
                            * _parameters.geometry.dx ) < distance ) {
                        distance = temp;
                    }
                    if ( ( temp = ( _parameters.parallel.firstCorner[0] + ( i - 2 + 0.5 ) )
                            * _parameters.geometry.dx ) < distance ) {
                        distance = temp;
                    }
                }

                flowField.getDistanceToNearWall().getScalar( i, j, k ) = distance;
            }
            // second special region
            if (i + _parameters.parallel.firstCorner[0] - 2 > xLimit &&
                    j + _parameters.parallel.firstCorner[1] - 2 < yLimit){
                wallOffset[0] = _parameters.parallel.firstCorner[0] + xLimit;
                FLOAT distance = ( _parameters.geometry.sizeY
                        - ( _parameters.parallel.firstCorner[1] + ( j - 2 + 0.5 ) ) )
                        * _parameters.geometry.dy;
                FLOAT temp;
                if ( ( temp = ( _parameters.parallel.firstCorner[1] + ( j - 2 + 0.5 ) )
                        * _parameters.geometry.dy ) < distance ) {
                    distance = temp;
                }
                if ( ( temp = ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) )
                        * _parameters.geometry.dz ) < distance ) {
                    distance = temp;
                }
                if ( ( temp = ( _parameters.geometry.sizeZ
                        - ( _parameters.parallel.firstCorner[2] + ( k - 2 + 0.5 ) ) )
                        * _parameters.geometry.dz ) < distance ) {
                    distance = temp;
                }

                    if ( ( temp = ( _parameters.geometry.sizeX
                            - ( wallOffset[0] + ( i - 2 + 0.5 ) ) )
                            * _parameters.geometry.dx ) < distance ) {
                        distance = temp;
                    }
                    if ( ( temp = (wallOffset[0] + ( i - 2 + 0.5 ) )
                            * _parameters.geometry.dx ) < distance ) {
                        distance = temp;
                    }

                flowField.getDistanceToNearWall().getScalar( i, j, k ) = distance;

            }
        }
    }
};

#endif /* INITNEARESTWALLSTENCIL_H_ */
