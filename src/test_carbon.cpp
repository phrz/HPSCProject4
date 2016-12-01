//
//  test_carbon.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/30/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <string>
#include "carbon.cpp"
#include "println.cpp"
#include "Vector.h"
#include "Matrix.h"

#ifndef _s
#define _s std::to_string
#endif

using namespace PH;

int main() {
	
	std::string prefix = "../data/";
	
	// All computations should use rtol = 10^−11 and atol = 10^−15.
	const double rtol = 1e-11;
	const double atol = 1e-15;
	
	
	// 1. Generate a linear span of 400 T (temperature in K)
	//    values over [800,1200].
	auto temperatureVals = Vector::linSpace(800, 1200, 400);
	temperatureVals.saveTo(prefix+"Temp.txt");
	
	
	// 2. Generate a linear span of 600 t (time in seconds) values from
	//    one second to two days.
	int twoDays = 2*24*60*60;
	auto timeVals = Vector::linSpace(1, twoDays, 600);
	timeVals.saveTo(prefix+"time.txt");
	
	
	// 3. For each depth [2mm, 4mm], do the following:
	for(int depth : {2, 4}) {
		
		// `x` must be in meters whereas `depth` is given in mm:
		double x = 1e-3*depth;
		
		// a. Generate a 400x600 matrix which evaluates C(0.004,t,T), with
		//    `T` spanning rows and `t` spanning columns.
		auto cMesh = Matrix(temperatureVals.size(), timeVals.size());
		
		cMesh.mapElements([&](double& cell, size_t r, size_t c) {
			cell = carbon(x, timeVals[c], temperatureVals[r], rtol, atol);
		});
		
		// b. Save this to "C%depth%mm.txt"
		cMesh.saveTo(prefix+"C"+_s(depth)+"mm.txt");
		
		// c. Separately, at each temperature
		//    [800K, 900K, 1000K, 1100K, and 1200K], do the following:
		for(int temp : {800, 900, 1000, 1100, 1200}) {
			
			//  i) evaluate concentration at a depth of 2mm (0.002m) over the above
			//     time value span.
			auto c = Vector(timeVals.size());
			
			c.mapElements([&](double& cell, size_t i) {
				cell = carbon(x, timeVals[i], (double)temp, rtol, atol);
			});
			
			// ii) save this array to `C%depth%mm_%temp%K.txt`
			std::string filename = prefix+"C"+_s(depth)+"mm_"+_s(temp)+"K.txt";
			c.saveTo(filename);
		}
	}
	
	return 0;
}
