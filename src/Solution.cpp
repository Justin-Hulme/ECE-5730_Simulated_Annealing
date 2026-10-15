#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <cstdlib>
#include <ctime>

#include "Solution.h"

Solution::Solution(const std::string& in_file_name){
    // open and parse the file
    parse_file(in_file_name);

    srand(time(0));
    // srand(1);

    // score the solution
    calculate_score();
}

Solution::~Solution() {
    for (auto& i : m_node_vec) {
        delete i;
        i = nullptr;
    }
}

Solution& Solution::operator=(const Solution& other){
    // check for self assignment
    if (this == &other){
        return *this;
    }

    // clear the old node vec
    for (auto& node : m_node_vec){
        delete node;
    }
    m_node_vec.clear();
    
    // clear the edge vector
    m_edge_vec.clear();

    copy_solution(other);

    return *this;
}

Solution::Solution(const Solution& other){
    copy_solution(other);
}

void Solution::copy_solution(const Solution& other){
    // copy simple ints
    m_score     = other.m_score;
    m_width     = other.m_width;
    m_height    = other.m_height;
    m_vert_num  = other.m_vert_num;
    m_index_a   = other.m_index_a;
    m_index_b   = other.m_index_b;

    // copy the node vector
    for (auto node : other.m_node_vec){
        m_node_vec.emplace_back(new Node(node->get_x(), node->get_y(), node->get_id()));
    }

    // recreate the edge vector
    for (auto edge : other.m_edge_vec){
        Node* from_node_ptr = nullptr;
        Node* to_node_ptr = nullptr;

        // find the from node and the to node
        for (auto& node : m_node_vec){
            if (node->get_id() == edge.from->get_id()){
                from_node_ptr = node;
            }

            if (node->get_id() == edge.to->get_id()){
                to_node_ptr = node;
            }
        }

        if (from_node_ptr && to_node_ptr){
            m_edge_vec.push_back({from_node_ptr, to_node_ptr});
        }
        else{
            std::cout << "something bad happened" << std::endl;
        }
    }
}

void Solution::modify(){
    // find a random node that is not empty
    int node_rand = rand() % m_vert_num;
    int num_nodes = 0;

    // loop through all the nodes
    for (int i = 0; i < static_cast<int>(m_node_vec.size()); i++) {
        Node* node = m_node_vec.at(i);
        
        // if the node is not empty
        if (node->get_id() != -1){            
            // if we have passed the random number of nodes
            if (num_nodes == node_rand){
                m_index_a = i;
                break;
            }

            // increment the number of nodes that we have seen
            num_nodes ++;
        }
    }
    
    // the second node can be any node including an empty one
    do {
        m_index_b = rand() % (m_width*m_height);
    } while (m_index_b == m_index_a);

    swap_nodes(m_index_a, m_index_b);
}

void Solution::unmodify(){
    swap_nodes(m_index_a, m_index_b);
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
                m_node_vec.emplace_back(new Node(i % m_width, i/m_width, i));
            } else {
                m_node_vec.emplace_back(new Node(i % m_width, i/m_width, -1));
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
            // from_node_ptr->connect_nodes(to_node_ptr);
            m_edge_vec.push_back({from_node_ptr, to_node_ptr});
        }
    }
}

void Solution::print_solution(){
    std::cout << "|";
    for (int x = 0; x < m_width; x++){
        std::cout << "---------|";
    }
    std::cout << std::endl;

    for (int y = 0; y < m_height; y++){
        std::cout << "|";

        for (int x = 0; x < m_width; x++){

            // print out the node if found or nothing if not
            if (m_node_vec.at(x+ m_width*y)->get_id() != -1){
                std::cout << std::setw(2) << m_node_vec.at(x+ m_width*y)->get_id() << " (" << m_node_vec.at(x+ m_width*y)->get_x() << ", " << m_node_vec.at(x+ m_width*y)->get_y() << ")";
            }
            else{
                std::cout << "         ";
            }

            std::cout << "|";
        }
        std::cout << std::endl << "|";

        for (int x = 0; x < m_width; x++){
            std::cout << "---------|";
        }

        std::cout << std::endl;
    }
}

void Solution::swap_nodes(const int& index_a, const int& index_b){
    Node* node_a_ptr = m_node_vec.at(index_a);
    Node* node_b_ptr = m_node_vec.at(index_b);

    int a_x = node_a_ptr->get_x();
    int a_y = node_a_ptr->get_y();

    // swap the nodes and fix their coordinates
    node_a_ptr->update_position(node_b_ptr->get_x(), node_b_ptr->get_y());  // set node a position to node b position
    node_b_ptr->update_position(a_x, a_y);                                  // set node b position to node a position

    // swap the indexes
    std::swap(m_node_vec.at(index_a), m_node_vec.at(index_b));

    calculate_score();
}