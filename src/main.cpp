#include <iostream>
#include <cmath>
#include <random>

#include "Solution.h"

#define INITIAL_TEMPERATURE 10000.0
#define COOLING_RATE 0.9999
#define TEMPERATURE_THRESHOLD 99.9

int main(int argc, char* argv) {
	//Begin

	// Assuming good behaving user (there is no error checking)
	std::string in_file_name = argv[1];
	std::string out_file_name = argv[2];

	// 	generate initial solution
	Solution solution(in_file_name);

	// 	score initial solution
	int solution_score = solution.get_score();

	// 	set initial temperature (T)
	double temperature = INITIAL_TEMPERATURE;

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, 100);
	int random_number = dist(rng);

	// Loop
	do {
	// 	generate new solution
		Solution new_solution = generate_new_solution(solution);

	// 	score new solution
		int new_solution_score = new_solution.get_score();

	// 	If new better than old
		if (new_solution_score > solution_score){
	// 		replace old solution with new
			solution = new_solution;

	// 	Else
		} else {
	// 		compute ΔE (ΔE = |scoreold − scorenew|)
			int delta_E = std::abd(old_solution_score - new_solution_score);

	// 		compute acceptance probability p = e^(-ΔE/T)
			acceptance_probability = std::exp(-delta_E / temperature);

	// 		generate random probability (r)
			random_number = dist(rng);

	// 		If (r ≤ p)
			if (random_number <= static_cast<int>acceptance_probability * 100) {
	// 			replace old solution with new
				solution = new_solution;
	// 		EndIf
			}
	// 	EndIf
		}

	// 	lower T
		temperature *= COOLING_RATE;

	// EndLoop when T is below threshold
	} while (T > TEMPERATURE_THRESHOLD);

	output_solution(solution, out_file_name);

	//End
	return 0;
}