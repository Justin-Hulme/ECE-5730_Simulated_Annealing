#include <iostream>
#include <cmath>
#include <random>

#include "Solution.h"

#define INITIAL_TEMPERATURE 100000.0
#define COOLING_RATE 0.999
#define TEMPERATURE_THRESHOLD 1

int main(int argc, char** argv) {
	//Begin
	(void) argc;
	(void) argv;

	// Assuming good behaving user (there is no error checking)
	std::string in_file_name("examples_inputs/input2.txt");
	std::string out_file_name("crap.txt");

	// 	generate initial solution
	Solution solution(in_file_name);
    std::cout << solution.get_score() << std::endl;
    // solution.print_solution();

	// 	set initial temperature (T)
	double temperature = INITIAL_TEMPERATURE;

	// set up random
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dist(0.0, 1.0);

    int i = 0;

	do {		
		// generate new solution
		Solution new_solution = solution;
		new_solution.modify();

		// If new solution is better than the old solution
		if (new_solution.get_score() < solution.get_score()){
			// replace old solution with new
			// new_solution.print_solution();
			// std::cout << "\n--------------------------------------------------------------\n" << std::endl;
            solution = new_solution;
			// solution.print_solution();
			// std::cout << std::endl << std::endl;
		} else {
			// compute ΔE (ΔE = |scoreold − scorenew|)
			int delta_E = std::abs(solution.get_score() - new_solution.get_score());

			// compute acceptance probability p = e^(-ΔE/T)
			double acceptance_probability = std::exp(-delta_E / temperature);

			// generate random probability (r)
			double random_double = dist(rng);

			// If (r ≤ p)
			if (random_double <= acceptance_probability) {
				// replace old solution with new
				// new_solution.print_solution();
				// std::cout << "\n--------------------------------------------------------------\n" << std::endl;
				solution = new_solution;
				// solution.print_solution();
				// std::cout << std::endl << std::endl;
			}
		}
		// 	lower T
		temperature *= COOLING_RATE;

        i++;

	// EndLoop when T is below threshold
	} while (temperature > TEMPERATURE_THRESHOLD);

	solution.export_solution(out_file_name);

    std::cout << solution.get_score() << " " << i << std::endl;
    solution.print_solution();

	return 0;
}