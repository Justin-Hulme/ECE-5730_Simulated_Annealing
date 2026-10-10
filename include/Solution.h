#pragma once

#include <vector>
#include <string>
#include "Node.h"

class Solution {
public:
    Solution(const std::string& in_file_name); // used to generate the 1st solution only
    Solution(std::vector<Node*> old_node_vec); // used to generate all the following solutions
    void update_solution(Solution new_solution);
    void calculate_score();
    int get_score();
    void export_solution(const std::string& file_name);
private:
    std::vector<Node*> m_node_vec;
    int m_score;
    int m_width;
    int m_height;
};