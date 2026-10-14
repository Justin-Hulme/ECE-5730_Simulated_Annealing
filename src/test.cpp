#include "Solution.h"
#include <iostream>

int main() {
    Solution sol("examples_inputs/input.txt");

    std::cout << "generated" << std::endl;

    sol.print_solution();
    sol.export_solution("crap.txt");
}