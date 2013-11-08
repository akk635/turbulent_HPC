#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

#include "Definitions.h"
#include <vector>
//#include "myVector.h"

/** Storage of a scalar field
 *
 * Parent of storage classes. Contains the data pointer and sizes in each
 * dimension.
 */
// Implement the DataType placeholder as a pointer so that it can have entities.
template<class DataType>
class Field
{
    private:
        // TODO WORKSHEET 1: put private member variable and function definitions here
		// 1-d data structure

    protected:
        // TODO WORKSHEET 1: put protected member variable and function definitions here
		int _x, _y, _z, entities;
    public:
        /** empty constructor for the field. This constructor is only provided for Worksheet 1
         *  to get the initial code compiled. It can be removed for future worksheets.
         */
	DataType* domain;
        Field():_x(), _y(), _z(),entities() {domain = NULL;}

        /** Constructor for the field
         *
         * General constructor. Takes the three arguments even if the matrix is
         * two dimensional. Doesn't allocate memory, only sets the dimensions
         *
         * @param Nx Number of cells in the x direction
         * @param Ny Number of cells in the y direction
         * @param Nz Number of cells in the z direction
         * @param components number of (vector) components per cell
         */
        Field ( int Nx, int Ny, int Nz, int components ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	_z = Nz;
        	entities = components;
        	domain = new DataType[ _x * _y * _z * entities ];
        }

        /** Destructor
         *
         * Deallocates the data for all the fields
         */
        virtual ~Field (){
          // TODO WORKSHEET 1
        	delete[] domain;
        }

        /** Returns the number of cells in the x direction
         *
         * @return The size in the x direction
         */
        int getNx () const {
          // TODO WORKSHEET 1
          return _x;
        }

        /** Returns the number of cells in the y direction
         *
         * @return The size in the y direction
         */
        int getNy () const {
          // TODO WORKSHEET 1
          return _y;
        }

        /** Returns the number of cells in the z direction
         *
         * @return The size in the z direction
         */
        int getNz () const {
          // TODO WORKSHEET 1
          return _z;
        }

        /** Index to array position mapper
         *
         * Index mapper. Converts the given index to the corresponding position
         * in the one-dimensional array. The index of the first component of the
         * respective cell vector should be returned by this method.
         *
         * @param i x index
         * @param j y index
         * @param k z index
         *
         * @return Position in the array
         */
        virtual int index2array ( int i, int j, int k = 0 ) const {

          // TODO WORKSHEET 1
          return ( ((k*_x*_y) + (j*_x) + i ) * entities ) ;
        }

};


/** Scalar field representation
 *
 * Stores a scalar field of floats. Derived from Field.
 */
class ScalarField: public Field<FLOAT>
{
    private:
        /** initializes the data of the scalar field by zeros. */
	FLOAT *scalarfield;
	Field<FLOAT> *field;
	void initialize(){
          // TODO WORKSHEET 1
		for( int i = 0; i < _x; i++){
			for( int j = 0; j < _y; j++){
				for( int k = 0; k < _z; k++){
					scalarfield[ this->index2array( i, j, k) ] = 0;
				}
			}
		}
        }

    public:

        /** 2D scalar field constructor.
         *
         * Sets the size of the data array and allocates data for the 2D field
         *
         * @param Nx Number of cells in direction x
         * @param Ny Number of cells in direction y
         */
        ScalarField ( int Nx, int Ny ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	entities = 1;
        	field = new Field<FLOAT> ( _x, _y, _z = 1, 1);
        	scalarfield = field->domain;
        	initialize();
        }

        /** 3D scalar field constructor.
         *
         * Sets the size of the data array and allocates data for the 3D field
         *
         * @param Nx Number of cells in direction x
         * @param Ny Number of cells in direction y
         * @param Nz Number of cells in direction z
         */
        ScalarField ( int Nx, int Ny, int Nz ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	_z = Nz;
        	entities = 1;
        	field = new Field<FLOAT> (_x, _y, _z, 1);
        	scalarfield = field->domain;
        	initialize();
        }

        ~ScalarField(){
        	delete field;
        }

        /** Acces to element in scalar field
         *
         * Returns a reference to an element of the scalar field, so that it
         * can be read and written.
         * @param i x index
         * @param j y index
         * @param k z index. Not required for arrays of dimension two.
         */
        FLOAT & getScalar ( int i, int j, int k = 0 ){
          // TODO WORKSHEET 1
        	if ((i < _x) && (j < _y) && (k < _z)){
        		return scalarfield[ index2array( i, j, k) ];
        	}
          static FLOAT invalidInitialValueForWorksheet1CodeFrame = -1;
          return invalidInitialValueForWorksheet1CodeFrame;
        }

        virtual int index2array ( int i, int j, int k = 0 ) const {
          // TODO WORKSHEET 1
          return ( ((k*_x*_y) + (j*_x) + i ) * entities ) ;
        }

        /** Prints the contents of the field
         *
         * Shows the content of the scalar field by printing them to stdout. Only clear if the
         * matrix is small enough. Used for debugging.
         *
         * @param title A label for the printed matrix
         */
        void show (const std::string title = ""){
          // TODO WORKSHEET 1
        }

};

/** Vector field representation
 *
 * Stores a vector field of floats. Derived from Field.
 */
class VectorField: public Field<FLOAT >
{
    private:
	FLOAT *vectorfield;
	Field<FLOAT> *field;
         /** initializes the data of the scalar field by zeros. */
        void initialize(){
           // TODO WORKSHEET 1
        	 // Already done by the default constructor of the class Vec23D
        	 for ( int i=0; i < _x; i++){
        		 for ( int j=0; j < _y; j++){
        			 for (int k=0; k < _z; k++){
        				 for (int e = 0; e < entities; e++){
        					 *(vectorfield + (this->index2array( i, j, k)) + e) = 0;
        				 }
        			 }
        		 }
        	 }
         }

    public:

        /** 2D Vector field constructor.
         *
         * Sets the size of the data array and allocates data for the 2D field
         *
         * @param Nx Number of cells in direction x
         * @param Ny Number of cells in direction y
         * @param Nz Number of cells in direction z
         */
        VectorField ( int Nx, int Ny ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	entities = 2;
        	field = new Field<FLOAT> (_x, _y, _z = 1, entities);
        	vectorfield = field->domain;
        	initialize();
        }

        /** 3D Vector field constructor.
         *
         * Sets the size of the data array and allocates data for the 3D field
         *
         * @param Nx Number of cells in direction x
         * @param Ny Number of cells in direction y
         * @param Nz Number of cells in direction z
         */
        VectorField ( int Nx, int Ny, int Nz ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	_z = Nz;
        	entities = 3;
        	field = new Field<FLOAT> (_x, _y, _z, entities);
        	vectorfield = field->domain;
        	initialize();
        }

        ~VectorField() {
        	delete field;
        }

        virtual int index2array ( int i, int j, int k = 0 ) const {
          // TODO WORKSHEET 1
          return ( ((k*_x*_y) + (j*_x) + i ) * entities ) ;
        }

        /** Non constant acces to an element in the vector field
         *
         * Returns a pointer to the position in the array that can be used to
         * modify it.
         *
         * @param i x index
         * @param j y index
         * @param k z index
         */
        FLOAT* getVector ( int i, int j, int k = 0 ){
          // TODO WORKSHEET 1
        	if ((i < _x) && (j < _y) && (k < _z)){
        		return &(vectorfield[ index2array( i, j, k) ]);
        	}
          return NULL;
        }
        /** Prints the contents of the field
         *
         * Shows the content of the first two components of a vector field by printing them to
         * stdout. Only clear if the matrix is small enough. Used for debugging.
         *
         * @param title A label for the printed matrix
         */
        void show (const std::string title = ""){
          // TODO WORKSHEET 1
        }
};


/** Integer field
 * 
 * Integer field with one value per position. Intended to represent flag
 * fields. Implemented because templates are undesirable at this point
 */
class IntScalarField : public Field<int> {

    private:
		int *flagfield;
		Field<int> *field;
        /** Initialize the array with zeros
         *
         * Had to be redeclared here because the object of the parent class
         * writes floats and caused a memory access violation.
         */
        void initialize (){
          // TODO WORKSHEET 1
        	for (int i=0; i < _x; i++){
        		for (int j=0; j < _y; j++){
        			for (int k=0; k < _z; k++){
        				flagfield[ this->index2array( i, j, k) ] = 0;
        			}
        		}
        	}
        }

    public:

        /** 2D constructor
         *
         * @param Nx Size in the x direction
         * @param Ny Size in the Y direction
         */
        IntScalarField ( int Nx, int Ny ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	entities = 1;
        	field = new Field<int> (_x, _y, _z = 1, 1);
        	flagfield = field->domain;
        	initialize ();
        }

        /** 3D constructor
         *
         * @param Nx Size in the x direction
         * @param Ny Size in the Y direction
         * @param Nz SIze in the Z direction
         */
        IntScalarField ( int Nx, int Ny, int Nz ){
          // TODO WORKSHEET 1
        	_x = Nx;
        	_y = Ny;
        	_z = Nz;
        	entities = 1;
        	field = new Field<int> (_x, _y, _z, 1);
        	flagfield = field->domain;
        	initialize ();
        }

        ~IntScalarField(){
        	delete field;
        }

        /** Access field values
         *
         * Returns a reference to the element with the given index
         *
         * @param i X index
         * @param j Y index
         * @param k Z index
         */
        int & getValue ( int i, int j, int k = 0 ){
          // TODO WORKSHEET 1
        	if ((i < _x) && (j < _y) && (k < _z)){
        		return flagfield[ index2array( i, j, k) ];
        	}
          static int invalidInitialValueForWorksheet1CodeFrame = -1;
          return invalidInitialValueForWorksheet1CodeFrame;

        }

        virtual int index2array ( int i, int j, int k = 0 ) const {
          // TODO WORKSHEET 1
          return ( ((k*_x*_y) + (j*_x) + i ) * entities ) ;
        }

};

#endif
