#pragma once

#include <vector>

class Node {
public:
    Node(int x, int y);
    void connect_nodes(Node* other_node);
    std::vector<Node*> get_connected();
    int distance_to(Node* other_node);
    void update_position(int x, int y);
private:
    int m_x;
    int m_y;
    std::vector<Node*> m_connected_nodes;
};