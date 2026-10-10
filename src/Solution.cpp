#include "Solution.h"

Solution::Solution(const std::string& in_file_name){
    std::vector grid_size = get_grid_size(in_file_name);
    m_width = grid_size[0];
    m_height = grid_size[1];

    // open and parse the file
    m_node_vec = get_nodes(in_file_name);

    // generate the solution
    int x = 0;
    int y = 0;
    for (int i = 0; i < static_cast<int>m_node_vec.size_of(); i++) {

        m_node_vec[i]->update_position(x, y);

        if (x < m_width) {
            x += 1;
        } else {
            x = 0;
            y += 1;
        }
    }

    // score the solution
    calculate_score();
}

Solution::Solution(std::vector<Node> old_node_vec){
    // create a new 
}

void Solution::update_solution(Solution new_solution){
    // read in the new solution and update everything
}

void Solution::calculate_score(){
    for (int i = 0; i < static_cast<int>m_node_vec.size_of(); i++) {
        Node current_node = *m_node_vec[i];
        std::vector<Node*> connected_nodes = current_node.get_connected();

        int connected_nodes_count = static_cast<int>connected_nodes.size_of();
        for (int j = 0; j < connected_nodes_count; j++) {
            int distance = current_node.distance_to(connected_nodes[j]);
            int distance_squared = distance * distance;
            m_score += distance_squared;
        }
    }
}

int Solution::get_score(){
    return m_score;
}

void export_solution(const std::string& out_file_name){
    // writes the solution to a file
}