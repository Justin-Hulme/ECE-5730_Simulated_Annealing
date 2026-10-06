#include <iostream>

#include "Solution.h"

#define INITIAL_TEMPERATURE 1000
#define COOLING_RATE 0.9999
#define TEMPERATURE_THRESHOLD

int main(int argc, char* argv) {
	// Begin
	// 	generate initial solution
	// 	score initial solution
	// 	set initial temperature (T)
	// 	Loop
	// 		generate new solution
	// 		score new solution
	// 		If new better than old
	// 			replace old solution with new
	// 		Else
	// 			compute ΔE (ΔE = |scoreold − scorenew|)
	// 			compute acceptance probability p = e^(-ΔE/T)
	// 			generate random probability (r)
	// 			If (r ≤ p)
	// 				replace old solution with new
	// 			EndIf
	// 		EndIf
	// 	lower T
	// 	EndLoop when T is below threshold
	// End
	return 0;
}