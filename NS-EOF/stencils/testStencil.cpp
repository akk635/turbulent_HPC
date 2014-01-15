/*
 * testStencil.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: karthik
 */
#include "testStencil.h"

testStencil::testStencil(){
    count = 0;
}

testStencil::~testStencil(){
}

// Transfer from right proc to left
void testStencil::applyTestrl( FlowField & flowField, int i, int j, int k ){
if ( j >= 2 && k >=2 ){
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
	fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
}else if ( j == 1 && k >= 2 ){
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
}
else if ( k == 1 && j >=2 ){
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
}
}

// Transfer from left proc to right
void testStencil::applyTestlr(FlowField & flowField, int i, int j, int k){
if ( j >= 2 && k >=2 ){
	fpVcomm << (flowField.getVelocity().getVector(i-1,j,k))[0] << "\n";
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
	fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
}
else if( j == 1 && k >= 2 ){
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
}
else if( k == 1 && j >= 2 ){
	fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
}
}

void testStencil::applyTesttb(FlowField & flowField, int i, int j, int k){
    if( i >= 2 && k >=2 ){
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
        fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
    }else if ((i == 1) & (k >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
    }else if ((k == 1) & (i >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
    }
}

void testStencil::applyTestbt(FlowField & flowField, int i, int j, int k){
    if( i >= 2 && k >=2 ){
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j-1,k))[1] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
        fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
    }else if ((i == 1) & (k >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
    }else if ((k == 1) & (i >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
    }
}

void testStencil::applyTestbf(FlowField & flowField, int i, int j, int k){
    if( i >= 2 && j >=2 ){
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
        fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
    }else if ((i == 1) & (j >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
    }else if ((j == 1) & (i >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
    }
}

void testStencil::applyTestfb(FlowField & flowField, int i, int j, int k){
    if( i >= 2 && j >=2 ){
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
        fpVcomm << (flowField.getVelocity().getVector(i,j,k-1))[2] << "\n";
        fpPcomm << flowField.getPressure().getScalar(i,j,k) << "\n";
    }else if ((i == 1) & (j >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
    }else if ((j == 1) & (i >= 2)) {
        fpVcomm << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
    }
}

void testStencil::applyFGHbnd (FlowField & flowField, int i, int j, int k){

	fpeqFGH << flowField.getFGH().getVector(i, j ,k)[1] << "\n";
	fpeqVbnd << flowField.getVelocity().getVector(i,j,k)[1] << "\n";

}

void testStencil::openfile(){
    std::string testVcomm = "Vcomm";
    std::string testPcomm = "Pcomm";
    std::string testFGH = "FGH";
    std::string testVel = "Vel";
    std::stringstream ss;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    ss << rank;
	count++;
	ss << count;
    testVcomm+=ss.str();
    testPcomm+=ss.str();
    testFGH+=ss.str();
    testVel+=ss.str();
    testVcomm+=".dat";
    testPcomm+=".dat";
    testFGH+=".dat";
    testVel+=".dat";
	fpVcomm.open(testVcomm.c_str());
	fpPcomm.open(testPcomm.c_str());
	fpeqFGH.open(testFGH.c_str());
	fpeqVbnd.open(testVel.c_str());
}

void testStencil::overwrite(){
	fpVcomm.close();
	fpPcomm.close();
	fpeqFGH.close();
	fpeqVbnd.close();
}

void testStencil::printItr( int j ){
	fpVcomm << 0 << j << "\n";
	fpPcomm << 0 << j << "\n";
	fpeqFGH << 0 << j << "\n";
	fpeqVbnd << 0 << j << "\n";
}


