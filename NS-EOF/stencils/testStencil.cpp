/*
 * testStencil.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: karthik
 */
#include "testStencil.h"

testStencil::testStencil(){

}

testStencil::~testStencil(){
	fpeq.close();
}

// Transfer from right proc to left
void testStencil::applyTest( FlowField & flowField, int i, int j, int k ){
if ( j >= 2 && k >=2 ){
	fpeq << (flowField.getVelocity().getVector(i,j,k))[0] << "\n";
	fpeq << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
	fpeq << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
	fpeq << flowField.getPressure().getScalar(i,j,k) << "\n";
}else if ( j == 1 && k >= 2 ){
	fpeq << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
}
else if ( k == 1 && j >=2 ){
	fpeq << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
}
}

// Transfer from left proc to right
void testStencil::applyTestlr(FlowField & flowField, int i, int j, int k){
if ( j >= 2 && k >=2 ){
	fpeq << (flowField.getVelocity().getVector(i-1,j,k))[0] << "\n";
	fpeq << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
	fpeq << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
	fpeq << flowField.getPressure().getScalar(i,j,k) << "\n";
}
else if( j == 1 && k >= 2 ){
	fpeq << (flowField.getVelocity().getVector(i,j,k))[1] << "\n";
}
else if( k == 1 && j >= 2 ){
	fpeq << (flowField.getVelocity().getVector(i,j,k))[2] << "\n";
}
}

void testStencil::applyFGHbnd (FlowField & flowField, int i, int j, int k){

	fpeqFGH << flowField.getFGH().getVector(i, j ,k)[0] << "\n";
	fpeqVbnd << flowField.getVelocity().getVector(i,j,k)[0] << "\n";

}

void testStencil::openfile(){
    std::string testevr = "eqV";
    std::string testFGH = "FGH";
    std::string testVel = "Vel";
    std::stringstream ss;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    ss << rank;
	count++;
	ss << count;
    testevr+=ss.str();
    testFGH+=ss.str();
    testVel+=ss.str();
    testevr+=".dat";
    testFGH+=".dat";
    testVel+=".dat";
	fpeq.open(testevr.c_str());
	fpeqFGH.open(testFGH.c_str());
	fpeqVbnd.open(testVel.c_str());
}

void testStencil::overwrite(){
	fpeq.close();
	fpeqFGH.close();
	fpeqVbnd.close();
}


