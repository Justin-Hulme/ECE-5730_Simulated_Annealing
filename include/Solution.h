#pragma once

#include <vector>
#include "Node.h"

class Solution {
public:
    Solution(char* file_name);
    Solution(std::vector<Node> old_node_vec);
    void update_solution(Solution new_solution);
    Solution generate_new_solution();
    int get_score();
    void export_solution(char* file_name);
private:
    std::vector<Node> m_node_vec;
    int m_score;
};