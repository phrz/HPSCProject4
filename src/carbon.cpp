//
//  carbon.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/30/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <cmath>
#include "println.cpp"
#include "adaptive_int.cpp"

// The temperature (Kelvin) dependent diffusion coefficient of steel
double diffusion(double T) {
	return 6.2e-7 * exp(-8.0e4/(8.31*T));
}

// The error function
double erf(const double y, const double rtol, const double atol) {
	// Use adaptive_int to evaluate the error function
	//
	// erf(y) = 2/sqrt(pi) I[0,y]( exp(-t^2) dt )
	//
	
	// the coefficient 2/sqrt(pi)
	static const double erfCoefficient = 1.1283791671;
	static Fcn erfIntegrand = [](double t){ return exp(-pow(t,2)); };
	
	int n, Ntot;
	double R;
	adaptive_int(erfIntegrand, 0.0, y, rtol, atol, R, n, Ntot);
	
	return R;
}

// Given x, t, and T, evaluate the carbon at depth concentration formula
// using the above erf() implementation.
double carbon(const double x, const double t, const double T,
			  const double rtol, const double atol)
{
	// the concentration of carbon C(x,t,T)
	// at a distance x (in meters) from the surface, at time t (in seconds),
	// for a specified temperature T (in Kelvin) is given by the formula:
	//
	//     C(x,t,T) = C_s - (C_s - C_0) erf( x/sqrt(4t D(T)) )
	//
	
	// C_0: Initial carbon concentration of a given steel (value given)
	static const double c0 = 0.001;
	
	// C_S: Carbon concentration of gaseous carburization medium
	static const double cS = 0.02;
	
	double erfV = x/sqrt(4.0*t * diffusion(T));
	return cS - (cS - c0) * erf(erfV, rtol, atol);
}
