#pragma once

#include <vector>
#include <string>
#include <random>
#include "Node.h"

struct Edge
{
    Node* from;
    Node* to;
};


class Solution {
public:
    Solution(const std::string& in_file_name); // used to generate the 1st solution only
    ~Solution();
    Solution& operator=(const Solution& other);
    Solution(const Solution& other);
    void copy_solution(const Solution& other);
    // Solution(std::vector<Node*> old_node_vec); // used to generate all the following solutions
    void modify();
    void unmodify();
    // justin void update_solution(Solution new_solution);
    void calculate_score();
    int get_score();
    void parse_file(const std::string& in_file_name);
    void export_solution(const std::string& file_name);
    void print_solution();
    void swap_nodes(const int& index_a, const int& index_b);
private:
    std::vector<Node*> m_node_vec;
    std::vector<Edge> m_edge_vec;
    int m_score;
    int m_width;
    int m_height;
    int m_vert_num;
    int m_index_a;
    int m_index_b;
};