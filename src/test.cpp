#include "Solution.h"
#include <iostream>

int main() {
    Solution sol("examples_inputs/input2.txt");

    std::cout << sol.get_score() << std::endl;

    sol.print_solution();
    sol.export_solution("crap.txt");
}