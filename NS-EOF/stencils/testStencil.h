/*
 * testStencil.h
 *
 *  Created on: Jan 7, 2014
 *      Author: karthik
 */

#ifndef TESTSTENCIL_H_
#define TESTSTENCIL_H_
#include "../FlowField.h"
#include <fstream>
#include <sstream>
#include <mpi.h>

class testStencil {
private:
	int rank;
public:
	std::ofstream fpeq;
	std::ofstream fpeqFGH;
	std::ofstream fpeqVbnd;
    int count = 0;
	testStencil();
	~testStencil();
	void openfile();
	void applyTest(  FlowField & flowField, int i, int j, int k);
	void applyTestlr(FlowField & flowField, int i, int j, int k);
	void applyFGHbnd(FlowField & flowField, int i, int j, int k);
	void overwrite();

};


#endif /* TESTSTENCIL_H_ */
