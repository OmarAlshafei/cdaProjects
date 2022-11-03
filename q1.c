// This is used to illustrate the basic gdb options for displaying values
// of variables and memory.
//
// Compile with:  gcc -o q1 -std=c11 -O0 -lm -Wall -W -ggdb3 q1.c
//
//    -std=c11 :  apply C11 Standard language features
//    -o q1    :  name the executable q1
//    -O0      :  do minimal optimizations of the code; good for debugging
//    -Wall    :  show warnings for the most common suspiscious things
//    -W       :  show warnings for less common suspiscious things
//    -lm      :  link in the floating-point math libraries
//    -ggdb3   :  build in extra code to help the debugger
//
// Invocation:  q1  a  b  c  x
//      where:    a, b, and c are coefficients of a quadratic polynomial func(),
//                   where func(x) = ax^2 + bx + c;
//                x is the value at which we want to analyze func();
//                all the parameters are real numbers
//
// The program computes func(z) and an approximation of func(z), using a tangent
// to extrapolate, for values z = x + k * stepSize, for k == 0 to k == stepLimit.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Declarations of constants that control where extrapolations are to be
// computed.
const int    stepLimit = 1000;     // number of extrapolation steps to compute
const double stepSize  = 0.001;    // increment applied at each step

// Declarations of helper functions used by main().  The compiler needs to see
// these declarations before it sees calls to the functions.
double func(double a, double b, double c, double x);
double funcPrime(double a, double b, double x);
double extrapolate(double a, double b, double c, double x, double h);
void writeExtrapolation(double xh, double approx, double direct);

int main(int argc, char** argv) {

   // check for the correct number of command-line parameters
   if ( argc != 5 ) {
      printf("Invocation: q1 a b c x\n");
      printf("   where a, b, and c are decimal values and a is not 0.\n");
      return 1;
   }
   
   // grab the command-line parameters; they are delivered to main() as an
   // array of C strings, but we want to interpret them as doubles
   double a = atof(argv[1]);    // atof() converts a character string to a double
   double b = atof(argv[2]);
   double c = atof(argv[3]);
   double x = atof(argv[4]);
   
   // make sure coefficient of x^2 isn't zero
   if ( a == 0.0 ) {
		printf("a must not be zero!\n");
		return 2;
	}

   // set initial step
   double h = 0.0;
   
   // compute some extrapolations of f(x + h)
   for (int i = 0; i < stepLimit; i++) {
		
		// compute the tangential approximation
		double approximation = extrapolate(a, b, c, x, h);
		
		// compute the actual function value
		double funcval = func(a, b, c, x + h);
		
		// print results to standard output
		writeExtrapolation(x + h, approximation, funcval);
		
		// set next step
		h = h + stepSize;
	}
	
   return 0;
}

/** Computes func(x) = ax^2 + bx + c.
 */
double func(double a, double b, double c, double x) {
	
	double fval = a;
	fval = fval * x + b;
	fval = fval * x + c;
	return fval;
}

/** Computes value of derivative of func() at x.
 */
double funcPrime(double a, double b, double x) {
	
	double slopeval = 2.0 * a;
	slopeval = slopeval * x + b;
	return slopeval;
}

/** Computes an approximation of func(x + h) by projecting the tangent
 *  at (x, f(x)).
 */
double extrapolate(double a, double b, double c, double x, double h) {
	
	double slope = funcPrime(a, b, x + h);
	double vchange = h * slope;
	double estimate = func(a, b, c, x) + vchange;
	return estimate;
}

/** Writes a nicely-formatted display of the results to standard output.
 */
void writeExtrapolation(double xh, double approx, double direct) {
	
	// A static local variable is initialized when the program starts up,
	// so firstCall will be true when the function is called the first time.
	// A static local variable then retains its last value, from call to 
	// call, so firstCall will be false on any subsequent calls.
	static bool firstCall = true;
	if ( firstCall ) {
		printf("  x + h      approximation    f(x + h)     error\n");
		printf("------------------------------------------------\n");
		firstCall = false;
	}
	
	// We'll print the three parameters so they align in nice columns,
	// with respect to the header printed by the code above.
	printf("%7.3f%19.3f%12.3f%10.3f\n", xh, approx, direct, direct - approx);
}
