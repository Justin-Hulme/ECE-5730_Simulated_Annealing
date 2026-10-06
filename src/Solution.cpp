#include "Solution.h"

Solution::Solution(char* file_name){
    // open parse the file
    // generate the solution
    // score the solution
}

Solution::Solution(std::vector<Node> old_node_vec){
    // create a new 
}

void Solution::update_solution(Solution new_solution){
    // read in the new solution and update everything
}

Solution Solution::generate_new_solution(){
    return Solution(m_node_vec);
}

int Solution::get_score(){
    return m_score;
}

void export_solution(char* file_name){
    // writes the solution to a file
}