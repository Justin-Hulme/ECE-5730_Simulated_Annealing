#pragma once

#include <vector>

class Node {
public:
    Node(const int& x, const int& y, const int& id);
    void connect_nodes(Node* other_node_ptr);
    std::vector<Node*> get_connected();
    int distance_to(Node* other_node_ptr);
    void update_position(int x, int y);
    int get_x();
    int get_y();
    int get_id();
private:
    int m_id;
    int m_x;
    int m_y;
    std::vector<Node*> m_connected_nodes;
};