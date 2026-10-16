#include <iostream>
#include <cmath>
#include <random>

#include "Solution.h"

#ifndef INITIAL_TEMPERATURE
	#define INITIAL_TEMPERATURE 1000000.0
#endif

#ifndef COOLING_RATE
	#define COOLING_RATE 0.9999
#endif

#ifndef TEMPERATURE_THRESHOLD
	#define TEMPERATURE_THRESHOLD 1.0
#endif


int main(int argc, char** argv) {
	// make sure we have the right number of arguments
	if (argc != 3){
        std::cout << "Too few arguments" << std::endl;
		return 0;
    }

	// get the file names
	std::string in_file_name(argv[1]);
	std::string out_file_name(argv[2]);

	// generate initial solution
	Solution old_solution(in_file_name);

	// set initial temperature (T)
	double temperature = INITIAL_TEMPERATURE;

	// set up random
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dist(0.0, 1.0);

	// track the number of iterations
    int iterations = 0;

	do {		
		// generate new solution
		Solution new_solution = old_solution;
		new_solution.modify();

		// if new solution is better than the old solution
		if (new_solution.get_score() < old_solution.get_score()){
			// replace old solution with new
            old_solution = new_solution;
		} else {
			// compute ΔE (ΔE = |scoreold − scorenew|)
			int delta_E = std::abs(old_solution.get_score() - new_solution.get_score());

			// compute acceptance probability p = e^(-ΔE/T)
			double acceptance_probability = std::exp(-delta_E / temperature);

			// generate random probability (r)
			double random_double = dist(rng);

			// if (r ≤ p)
			if (random_double <= acceptance_probability) {
				old_solution = new_solution;
			}
		}
		// lower T
		temperature *= COOLING_RATE;

		// increment the number of iterations
        iterations++;

	// endLoop when T is below threshold
	} while (temperature > TEMPERATURE_THRESHOLD);

	// export the solution
	old_solution.export_solution(out_file_name);

#ifdef DEBUG
    std::cout << old_solution.get_score() << " " << iterations << std::endl;
    solution.print_solution();
#endif

	return 0;
}