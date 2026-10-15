#include <iostream>
#include <cmath>
#include <random>

#include "Solution.h"


int main(int argc, char** argv) {
	if (argc != 6){
        std::cout << "you done messed up" << std::endl;
    }

	// Assuming good behaving user (there is no error checking)
	std::string in_file_name(argv[1]);
	std::string out_file_name(argv[2]);

    double INITIAL_TEMPERATURE = std::stod(argv[3]);
    double COOLING_RATE        = std::stod(argv[4]);
    double TEMPERATURE_THRESHOLD = std::stod(argv[5]);

	// generate initial solution
	Solution solution(in_file_name);

	// set initial temperature (T)
	double temperature = INITIAL_TEMPERATURE;

	// set up random
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<> dist(0.0, 1.0);

    int i = 0;

	do {		
		// generate new solution
		Solution new_solution = solution;
		new_solution.modify();

		// if new solution is better than the old solution
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

			// if (r ≤ p)
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

	// endLoop when T is below threshold
	} while (temperature > TEMPERATURE_THRESHOLD);

	solution.export_solution(out_file_name);

#ifdef DEBUG
    std::cout << solution.get_score() << " " << i << std::endl;
    solution.print_solution();
#endif

	return 0;
}