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

// Use adaptive_int() to approximate the error function `erf` within given
// tolerances
double erf(const double y, const double rtol, const double atol) {
	// Use adaptive_int to evaluate the error function
	//
	// erf(y) = 2/sqrt(pi) I[0,y]( exp(-t^2) dt )
	//
	
	// the coefficient 2/sqrt(pi)
	static const double erfCoefficient = 1.1283791671;
	static Fcn erfIntegrand = [](double t){ return exp(-pow(t,2)); };
	
	int n, Ntot;
	double integral;
	int error = adaptive_int(erfIntegrand, 0.0, y, rtol, atol, integral, n, Ntot);
	
	if(error != 0) {
		println("[erf] integral approximation failed during evaluation of the error function.");
		return 0;
	}
	
	return erfCoefficient * integral;
}

// Given x, t, and T, evaluate the carbon at depth concentration formula
// using the above erf() implementation.
double carbon(const double x, const double t, const double T,
			  const double rtol, const double atol)
{
	// Considering the carburization process, given a sample of steel of known
	// initial carbon concentration, a gas of known carbon concentration, and
	// a constant environmental temperature, calculate the carbon concentration
	// at a certain depth within the steel at a given time during the process.
	
	// KNOWN:
	// - C_0, the initial concentration of carbon uniformly throughout the steel
	//   sample
	static const double c0 = 0.001;
	
	// - C_S, the concentration of carbon in the gaseous carburization medium
	static const double cS = 0.02;
	
	// GIVEN:
	// - x, the distance in meters from the surface of the steel sample
	// - t, the time in seconds from the beginning of the carburization
	// - T, the constant temperature in Kelvin at which the process is occurring
	// - rtol and atol, the tolerances for error function integral approximation
	
	// EXPECTED BEHAVIOR:
	// - the return value is the concentration within the steel at depth `x`,
	//   at time `t`, in a carburizing process at temperature `T`.
	// - if the error function cannot be approximated by the `adaptive_int`
	//   algorithm in erf(), the return value is undefined.
	
	// FORMULA:
	//
	//     C(x,t,T) = C_s - (C_s - C_0) erf( x/sqrt(4t D(T)) )
	//
	
	double erfV = x/sqrt(4.0*t * diffusion(T));
	return cS - (cS - c0) * erf(erfV, rtol, atol);
}
