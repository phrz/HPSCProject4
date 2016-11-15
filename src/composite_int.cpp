//
//  composite_int.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/15/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <iostream>
#include <exception>
#include <cmath>
#include "Vector.h"

using namespace PH;
using Fcn = std::function<double(double)>;

// Integrate function `f` on the interval `a` to `b`
// with `n` subintevals of width |b-a|/n.
double composite_int(Fcn& f, const double a, const double b, const int n) {
	// implement a composite numerical integration formula that is
	// O(h^8) accurate.
	
	// PRECONDITIONS:
	// (1) [a,b] is a valid, forward interval.
	if(b < a) {
		throw std::invalid_argument("The interval [a,b] is invalid, b < a.");
	}
	
	// (2) `n` is a nonzero quantity (integer)
	if(n < 1) {
		throw std::invalid_argument("The number of nodes is less than one, n < 1.");
	}
	
	// IMPLEMENTATION: 4-node Gaussian quadrature
	//
	//  node nº |       offset               |        weight        |
	// =========+============================+======================+
	//     1    | +sqrt(3/7 - 2/7*sqrt(6/5)) | (18 + sqrt(30))/36   |
	//     2    | -sqrt(3/7 - 2/7*sqrt(6/5)) | (18 + sqrt(30))/36   |
	//     3    | +sqrt(3/7 + 2/7*sqrt(6/5)) | (18 - sqrt(30))/36   |
	//     4    | -sqrt(3/7 + 2/7*sqrt(6/5)) | (18 - sqrt(30))/36   |
	// =========+============================+======================+
	
	// pre-calculated using the above expressions, which were taken from
	// a table in the Wikipedia article on Gaussian quadrature.
	const Vector offsets = {0.11558710999704797, -0.11558710999704797,
							0.7415557471458092 , -0.7415557471458092};
	
	const Vector weights = {0.6521451548625462 , 0.6521451548625462,
							0.34785484513745385, 0.34785484513745385};
	
	
	
	return -1.0;
}
