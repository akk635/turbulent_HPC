/*
 * myVector.h
 *
 *  Created on: Oct 30, 2013
 *      Author: karthik
 */

#ifndef MYVECTOR_H_
#define MYVECTOR_H_

#include "Definitions.h"

template <typename T>
class Vec23D {
	private:
	// Components in each directions
		T x, y, z;
	public:
		// Default Constructor
		Vec23D(): x(), y(), z() {};

		// 2d Constructor for the vector allocation
		Vec23D(T xcomp, T ycomp): x(xcomp), y(ycomp){};

		// 3d Constructor for the vector
		Vec23D(T xcomp, T ycomp, T zcomp): x(xcomp), y(ycomp), z(zcomp){};

		// Operator overloading of the various operations
	    Vec23D& operator=(const Vec23D& other) {
	        x = other.x;
	        y = other.y;
	        return *this;
	    }
	    Vec23D& operator+=(const Vec23D& other) {
	        x += other.x;
	        y += other.y;
	        return *this;
	    }
	    Vec23D& operator-=(const Vec23D& other) {
	        x -= other.x;
	        y -= other.y;
	        return *this;
	    }
	    Vec23D& operator*=(const T& scalar) {
	        x *= scalar;
	        y *= scalar;
	        return *this;
	    }
	    Vec23D& operator/=(const T& scalar) {
	        x /= scalar;
	        y /= scalar;
	        return *this;
	    }

	    // If the vector is not to be changed
	    Vec23D operator*(const T& scalar) const {
	        Vec23D tmp = *this;
	        tmp *= scalar;
	        return tmp;
	    }
	    Vec23D operator/(const T& scalar) const {
	        Vec23D tmp = *this;
	        tmp /= scalar;
	        return tmp;
	    }
	    Vec23D operator+(const Vec23D& other) const {
	        Vec23D tmp = *this;
	        tmp += other;
	        return tmp;
	    }
	    Vec23D operator-(const Vec23D& other) const {
	        Vec23D tmp = *this;
	        tmp -= other;
	        return tmp;
	    }
	    friend std::ostream& operator<< <>(std::ostream &dest,
	                                      const Vec23D<T>& vec);
	    friend std::istream& operator>> <>(std::istream &src,
	                                      Vec23D<T>& vec);
};

template <typename T>
std::ostream& operator<<(std::ostream &dest,
                                const Vec23D<T>& vec) {
    dest << vec.x << " " << vec.y << " " <<vec.z<<"\n";
    return dest;
}

template <typename T>
std::istream& operator>>(std::istream &src,
                                  Vec23D<T>& vec) {
    src >> vec.x >> vec.y >> vec.z;
    return src;
}

#endif /* MYVECTOR_H_ */
