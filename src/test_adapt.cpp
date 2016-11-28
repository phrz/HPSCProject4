//
//  test_adapt.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/16/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <vector>
#include <cmath>
#include "adaptive_int.cpp"
#include "println.cpp"

using namespace std;

int main() {
	
	// Evaluate the same problem as `test_int`:
	
	// limits of integration
	double a = -3.0;
	double b = 5.0;
	
	double c = 0.5;
	double d = 25.0;
	
	// function
	Fcn f = [&](double x) {
		return (exp(c*x) + sin(d*x));
	};
	
	// antiderivative
	Fcn F = [&](double x) {
		return (exp(c*x)/c - cos(d*x)/d);
	};
	
	// Let:
	//
	//     rtol = {10^-2, 10^-4, 10^-6 ..., 10^-12}
	//     atol_i = rtol_i/1000
	//
	// For each set of tolerances, print |I(f)-R(f)| and rtol*|I(f)|+atol,
	// as well as `n` and `Ntot`.
	
	size_t t_s = 6;
	vector<double> rtol(t_s);
	vector<double> atol(t_s);
	
	// Iterate once to build rtol and atol.
	for(int i = 0; i < t_s; ++i) {
		rtol[i] = std::pow(10.0, -2*(i+1));
		atol[i] = rtol[i]/1000;
	}
	
	size_t computationalCost = 0;

	// Iterate again to test `adaptive_int`.
	for(int i = 0; i < 6; ++i) {
		println("rtol: ",rtol[i],"\tatol:",atol[i]);
		
		int Ntot, n;
		double R;
		int status = adaptive_int(f, a, b, rtol[i], atol[i], R, n, Ntot);
		
		if(status != 0) {
			println("FAILURE!");
		} else {
			println("n:",n,"\tNtot:",Ntot,"\tR:",to_string(R),"\n\n");
			computationalCost += Ntot;
		}
	}
	
	println("Total computational cost of these tests:", computationalCost);
	
	return 0;
}
