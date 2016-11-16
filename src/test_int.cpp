//
//  test_int.cpp
//  HPSCProject4
//
//  Created by Paul Herz on 11/15/16.
//  Copyright © 2016 Paul Herz. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <experimental/optional>
#include <string>
#include <sstream>

#include "println.cpp"
#include "composite_int.cpp"

template<class T>
using optional = std::experimental::optional<T>;

using namespace std;

string leftPad(string s, size_t size, char pad = ' ') {
	size_t difference = size - s.size();
	return string(difference, pad) + s;
}

string rightPad(string s, size_t size, char pad = ' ') {
	size_t originalSize = s.size();
	if(originalSize == size) {
		return s;
	}
	
	// either truncate or pad the string depending
	// on requested size vs actual size.
	s.resize(size, pad);
	
	// if shrunken, replace last character with
	// ellipsis character.
	if(originalSize > size) {
		s.replace(originalSize-1, 1, "…");
	}
		
	return s;
}

template<typename T>
string leftPad(T t, size_t size, char pad = ' ') {
	stringstream stream;
	stream << t;
	auto s = stream.str();
	return leftPad(s, size, pad);
}

template<typename T>
string rightPad(T t, size_t size, char pad = ' ') {
	stringstream stream;
	stream << t;
	auto s = stream.str();
	return rightPad(s, size, pad);
}

int main(int argc, char* argv[]) {
	
	// Using the same integrand as `test_Gauss2.cpp`, modify `n`
	// to demonstrate the convergence rate of the method with n >= 6.
	// Include this output in the report.
	
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
	
	double trueIntegral = F(b) - F(a);
	println("True integral:", trueIntegral);
	
	vector<int> n = {20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240};
	vector<double> errors(n.size());
	vector<double> hvals(n.size());
	optional<double> convergence;
	
	println("┌────────┬────────────┬─────────────┬───────────────┐");
	println("|      n |   approx   |    error    |  convergence  |");
	println("├────────┼────────────┼─────────────┼───────────────┤");
	
	for(size_t i = 0; i < n.size(); ++i) {
		double approximateIntegral = composite_int(f, a, b, n[i]);
		errors[i] = abs(trueIntegral-approximateIntegral)/abs(trueIntegral);
		hvals[i] = (b-a)/n[i];
		
		if(i != 0) {
			convergence = (log(errors[i-1]) - log(errors[i]))
						/ (log( hvals[i-1]) - log( hvals[i]));
		}
		
		string conv = "n/a";
		if(convergence) {
			conv = to_string(*convergence);
		}
		
		println("│ " + leftPad(n[i],6)
		  +" │ "+ rightPad(approximateIntegral,11)
		  +"│ " + rightPad(errors[i],12)
		  +"│ " + rightPad(conv, 14)
		  +"│");
	}
	
	println("└────────┴────────────┴─────────────┴───────────────┘");
	
	return 0;
}
