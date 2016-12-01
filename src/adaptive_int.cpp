//
//  adaptive_int.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/16/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <cmath>
#include "println.cpp"
#include "composite_int.cpp"

using Fcn = std::function<double(double)>;

// Call `composite_int` adaptively, such that
//
//     abs( I(f) - Rn(f) ) < rtol * abs( I(f) ) + atol
//
// Where I(f) is the integral, and Rn(f) is the `composite_int` function,
// and computation is minimized to achieve the above condition.
//
// Because the true integral is not known, we can use the rapid convergence
// of `composite_int` to approximate I(f) in a way that helps find `n`.
// Beginning with k at an arbitrary value, update `n` until this
// approximation is satisfied:
//
//     abs( Rn+k(f) - Rn(f) ) < rtol * abs( Rn+k(f) ) + atol

template<typename T>
int adaptive_int(T& f, const double a, const double b, const double rtol,
				 const double atol, double& R, int& n, int& Ntot)
{
	// Compute the integral of f(x) over [a,b] relative to x adaptively,
	// meeting an error bound condition while minimizing computation.
	
	// GIVEN:
	// - [a,b], the integral bounds
	// - rtol and atol, parameters of the error condition,
	// - other parameters solely for output (R, n, Ntot)
	
	// EXPECTED BEHAVIOR:
	// - `R` should be assigned an approximation.
	// - `n` should be assigned the subinterval count that first satisfied
	//   the error threshold in the iterative ('adaptive') process.
	// - `Ntot` should be assigned the total number of subintervals evaluated
	//   before and including a calculation satisfying the error condition.
	// - The return value should be '0' if successful in satisfying the error
	//   condition, and '1' otherwise.
	
	// SIDE EFFECTS:
	// - `R`, `n`, and `Ntot` are guaranteed to be modified under successful
	//   conditions.
	// - Under failing conditions, `Ntot` will either be zero or an undefined
	//   value.
	// - Under failing conditions, `n` will be an undefined value.
	// - Under failing conditions, `Ntot` will be zero or an undefined value.
	
	// STRATEGY: with an step counter i=0, n begins at 6, k = floor((i+1)^2)
	// (i) Initialize return parameters
	
	// current integral approximation
	R = 0;
	
	// total subintervals evaluated
	Ntot = 0;
	
	// subintervals to evaluate on this step
	n = 6;
	
	// (ii) Initialize other variables for calculation
	
	// step counter for iterative adaptation
	int i = 0;
	int iMax = 1e6;
	
	// The next integral approximation after `R`, where `R` has `n` subintervals
	// and `Rnext` has `n+k` subintervals - used to approximate I(f).
	double Rnext = 0.0;
	
	// The flag used to continue iteration
	bool underThreshold = false;
	
	// Absolute error function
	auto error = [&](double a1, double a2) -> double {
		return std::abs(a1-a2);
	};
	
	// Upper bound condition for success given an integral I(f) or
	// in this case an integral approximation.
	auto bound = [&](double I) -> double {
		return rtol * std::abs(I) + atol;
	};
	
	while(!underThreshold && i <= iMax) {
		
		int nextN = 1.5*n+10;
		
		// (a) Evaluative step: determine if error condition is satisfied.
		
		// At i=0, calculate Rn and Rn+k. After that, shift Rn := Rn+k,
		// then calculate just Rn+k, because n := n+k at the iterative step.
		//
		// i.e.:
		//     i=0 -> 2n+k subinterval calculations,
		//     i≠0 -> n+k subinterval calculations.
		//
		
		if(i != 0) {
			R = Rnext;
		} else {
			// Calculate Rn once at i=0, and record nº of subintervals to Ntot.
			R = composite_int(f, a, b, n);
			Ntot += n;
		}
		
		// Calculate Rn+k for all i, and record nº of subintervals to Ntot.
		Rnext = composite_int(f, a, b, nextN);
		Ntot += nextN;
		
		underThreshold = (error(Rnext, R) < bound(Rnext));
		
		if(underThreshold) {
			// All returning variables, `Ntot`, `n`, and `R` are set.
			// Just exit before iterative step.
			return 0;
		}
		
		// (b) Iterative step: increase `n` by a heuristic amount
		n = nextN;
	}
	
	return 1;
}
