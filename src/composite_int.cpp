//
//  composite_int.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/15/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#ifndef _s
#define _s std::to_string
#endif

#include <iostream>
#include <exception>
#include <cmath>

using Fcn = std::function<double(double)>;

// Integrate function `f` on the interval `a` to `b`
// with `n` subintevals of width |b-a|/n.

// NOTE: using template instead of Fcn directly to avoid the overhead of
// std::function, see:
// http://blog.demofox.org/2015/02/25/avoiding-the-performance-hazzards-of-stdfunction/

template<typename T>
double composite_int(T& f, const double a, const double b, const int n) {
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
	
	// offset values (given)
	static const double o[] = {0.11558710999704797, -0.11558710999704797,
							   0.7415557471458092 , -0.7415557471458092};
	
	// weights (given)
	static const double w[] = {0.6521451548625462 , 0.6521451548625462,
						       0.34785484513745385, 0.34785484513745385};
	
	// subinterval width equals the interval width,
	// divided by nº of subintervals `n`.
	
	const double h = (b-a)/n;
	const double h2 = h/2.0;
	
	double result = 0.0;
	double mid = 0.0;
	
	// result = h/2 * sum[i=0...n-1](s(i)), where s(i) is
	// the integral of the function f(x) over the i-th subinterval w/r to x.
	
	for(int i = 0; i < n; ++i) {
		
		// calculate the midpoint for the i-th subinterval [a+i(h),a+i(h+1)]
		mid = a + (i + 0.5) * h;
		
		// Evaluate the integral over this i-th subinterval using the 4-node
		// Gaussian quadrature method, i.e:
		//
		// s(i) = I[a+ih,a+i(h+1)] f(x)dx = sum[k=0...3] w_k * f(nodes_k)
		//
		result += w[0] * f(o[0]*h2+mid)
			    + w[1] * f(o[1]*h2+mid)
				+ w[2] * f(o[2]*h2+mid)
				+ w[3] * f(o[3]*h2+mid);
	}
	
	result *= h2;
	return result;
}
