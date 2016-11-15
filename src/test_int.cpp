//
//  test_int.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/15/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "println.cpp"
#include "composite_int.cpp"

using namespace std;

int main(int argc, char* argv[]) {
	
	// Using the same integrand as `test_Gauss2.cpp`, modify `n`
	// to demonstrate the convergence rate of the method with n >= 6.
	// Include this output in the report.
	
	Fcn f = [](double x){ return x*x; };
	
	println(composite_int(f, 0, 1, 6),"\n");
	
	return 0;
}
