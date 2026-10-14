#include <fstream>
#include <iostream>
#include <iomanip>

#include "Solution.h"

Solution::Solution(const std::string& in_file_name)
    : m_rng(std::random_device{}(),
      node_dist(),
      grid_dist()
{
    // open and parse the file
    parse_file(in_file_name);

    // setup random number distribution
    m_rng(std::random_device{}());
    node_dist = std::uniform_int_distribution<>(0, m_vert_num);
    grid_dist = std::uniform_int_distribution<>(0, m_width*m_height - 1);

    // score the solution
    calculate_score();
}

Solution::~Solution(){
    for (auto i : m_node_vec){
        delete i;
    }  
}

void Solution::modify(){
    // get 2 random numbers
    node_rand = node_dist(m_rng);   // bounded by actual node count
    grid_rand = grid_dist(m_rng);   // bounded by total grid size

    // find the node associated with the first number
    int index_a;
    for (int i = 0; i < static_cast<int>(m_node_vec.size());) {
        if (m_node_vec.at(i).get_id() >= 0) {
            if (i == node_rand) {
                Node node_a = m_node_vec.at(i);
                index_a = i;
                break;
            }
            i++;
        }
    }

    // find the node associated with the second number
    for (int index_b = 0; i < m_width*m_height - 1; i++) {
        if (i == grid_rand) {
            Node node_b = m_node_vec.at(i);
            break;
        }
    }

    // swap the nodes and fix their coordinates
    node_a.update_position(m_node_vec.at(index_b).get_x(), m_node_vec.at(index_b).get_y());
    node_b.update_position(m_node_vec.at(index_a).get_x(), m_node_vec.at(index_a).get_y());

    m_node_vec.at(index_a) = node_b;
    m_node_vec.at(index_b) = node_a;
}

void Solution::calculate_score(){
    int score = 0;

    // loop through all the edges
    for (auto edge : m_edge_vec) {
        // calculate the distance squared       
        int distance = edge.from->distance_to(edge.to);
        int distance_sq = distance * distance; 
        
        // update the score
        score += distance_sq;
    }

    // save the score
    m_score = score;
}

int Solution::get_score(){
    return m_score;
}

void Solution::export_solution(const std::string& out_file_name){
    // open file
    std::ofstream out_file(out_file_name);

    if (out_file.is_open()){
        // print the nodes in order
        for (int i = 0; i < m_vert_num; i++){
            // find the correct node
            for (auto node : m_node_vec){
                if (node->get_id() == i){
                    // add it to the file
                    out_file << "Node " << i << " placed at (" << node->get_x() << ", " << node->get_y() << ")\n";
                }
            }
        }

        // loop through all the edges
        for (auto edge : m_edge_vec){
            // print the values
            out_file << "Edge from " << edge.from->get_id() << " to " << edge.to->get_id() << " has length " << edge.from->distance_to(edge.to) << "\n";
        }

        // close the file
        out_file.close();
    }
}

void Solution::parse_file(const std::string& in_file_name){
    std::ifstream file(in_file_name);

    if (file.is_open()){
        std::string grid_size_s, vert_num_s, blank;
        
        // get the lines from the static top of the file
        std::getline(file, grid_size_s);
        std::getline(file, vert_num_s);
        std::getline(file, blank);
        
        // parse the first two lines of the file
        sscanf(grid_size_s.c_str(), "g %d %d", &m_width, &m_height);
        sscanf(vert_num_s.c_str(), "v %d", &m_vert_num);

        // build the node vector
        for (int i = 0; i < m_width * m_height; i++){
            if (i < m_vert_num) {
                // add the nodes in order, left to right and top to bottom
                m_node_vec.emplace_back(new Node(i % m_width, i/m_height, i));
            } else {
                m_node_vec.emplace_back(new Node(i % m_width, i/m_height, -1));
            }
        }

        // parse the rest of the file and build the graph
        std::string line;
        while (std::getline(file, line)){
            int from_node_pos, to_node_pos;

            // read the line
            sscanf(line.c_str(), "e %d %d", &from_node_pos, &to_node_pos);

            // find the references to the nodes
            Node* from_node_ptr = m_node_vec.at(from_node_pos);
            Node* to_node_ptr = m_node_vec.at(to_node_pos);

            // connect the nodes
            from_node_ptr->connect_nodes(to_node_ptr);
            m_edge_vec.push_back({from_node_ptr, to_node_ptr});
        }
    }
}

void Solution::print_solution(){
    std::cout << "|";
    for (int x = 0; x < m_width; x++){
        std::cout << "-----|";
    }
    std::cout << std::endl;

    for (int y = 0; y < m_height; y++){
        std::cout << "|";

        for (int x = 0; x < m_width; x++){

            // print out the node if found or nothing if not
            if (m_node_vec.at(x+ m_width*y)->get_id() != -1){
                std::cout << std::setw(5) << m_node_vec.at(x+ m_width*y)->get_id();
            }
            else{
                std::cout << "     ";
            }

            std::cout << "|";
        }
        std::cout << std::endl << "|";

        for (int x = 0; x < m_width; x++){
            std::cout << "-----|";
        }

        std::cout << std::endl;
    }
}