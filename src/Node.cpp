#include "Node.h"
#include <cmath>

Node::Node(const int& x, const int& y, const int& id){
    m_x = x;
    m_y = y;
    m_id = id;
}

int Node::distance_to(Node* other_node_ptr){
    // get the grid distance
    return abs(m_x - other_node_ptr->m_x) + abs(m_y - other_node_ptr->m_y);
}

void Node::update_position(int x, int y){
    m_x = x;
    m_y = y;
}

int Node::get_x(){
    return m_x;
}

int Node::get_y(){
    return m_y;
}

int Node::get_id(){
    return m_id;
}