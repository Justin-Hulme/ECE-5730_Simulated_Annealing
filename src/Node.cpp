#include "Node.h"
#include <cmath>

Node::Node(int x, int y){
    m_x = x;
    m_y = y;
}

void Node::connect_nodes(Node* other_node){
    m_connected_nodes.push_back(other_node);
}

std::vector<Node*> Node::get_connected(){
    return m_connected_nodes;
}

int Node::distance_to(Node* other_node){
    return abs(m_x - other_node->m_x) + abs(m_y - other_node->m_y);
}

void Node::update_position(int x, int y){
    m_x = x;
    m_y = y;
}

int Node::get_x(){
    return m_x;
}

int Node::get_y(){
    return m_x;
}
