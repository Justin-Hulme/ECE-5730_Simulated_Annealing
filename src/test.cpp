#include <iostream>
#include <cmath>
#include <random>

#include "Solution.h"

#define INITIAL_TEMPERATURE 1000000.0
#define COOLING_RATE 0.9999
#define TEMPERATURE_THRESHOLD 99.9

int main(int argc, char** argv) {
	//Begin
	(void) argc;

	// Assuming good behaving user (there is no error checking)
	std::string in_file_name("examples_inputs/input.txt");
	std::string out_file_name("crap.txt");

	// 	generate initial solution
	Solution solution(in_file_name);
    std::cout << solution.get_score() << std::endl;
    // solution.print_solution();

	// 	score initial solution
	int solution_score = solution.get_score();

	// 	set initial temperature (T)
	double temperature = INITIAL_TEMPERATURE;

	// set up random
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dist(0.0, 1.0);

    int i = 0;

	// Loop
	do {
	// 	generate new solution
		// Solution new_solution = solution;

		solution_score = solution.get_score();
		solution.modify();
        
        // 	score new solution
		int new_solution_score = solution.get_score();
        
        // std::cout << "\n" << new_solution_score << " " << solution_score << "--------------------------------------------------\n\n";
        // solution.print_solution();

	// 	If old better than new
		if (new_solution_score > solution_score){
	// 		replace old solution with new
            solution.unmodify();
            
	// 	Else
		} else {
	// 		compute ΔE (ΔE = |scoreold − scorenew|)
			int delta_E = std::abs(solution_score - new_solution_score);

	// 		compute acceptance probability p = e^(-ΔE/T)
			double acceptance_probability = std::exp(-delta_E / temperature);

	// 		generate random probability (r)
			double random_double = dist(rng);

	// 		If (r ≤ p)
			if (random_double > acceptance_probability) {
	// 			replace old solution with new
				solution.unmodify();
	// 		EndIf
			}
	// 	EndIf
		}

	// 	lower T
		temperature *= COOLING_RATE;

        i++;

	// EndLoop when T is below threshold
	} while (temperature > TEMPERATURE_THRESHOLD);

	solution.export_solution(out_file_name);

    std::cout << solution.get_score() << " " << i << std::endl;
    solution.print_solution();

	//End
	return 0;
}