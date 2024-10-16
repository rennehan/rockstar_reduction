#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <memory>
#include <set>
#include "main.hpp"
#include "tree/Node.hpp"

// depth first search
void open_node(const std::shared_ptr<Node> &node_to_visit, std::shared_ptr<std::set<int64_t>> &set) {
    // we made it back to the top of the tree with no more children to visit!
    if (node_to_visit == nullptr) {
        return;
    }

    // check if the node ID is in the stack
    if (set->find(node_to_visit->halo.get_id()) == set->end()) {
        std::cout << node_to_visit->halo.get_id() << ", ";
        set->insert(node_to_visit->halo.get_id());
    }

    // visit all the children of this node
    for (auto &child : node_to_visit->children_) {
        if (set->find(child->halo.get_id()) == set->end()) {
            open_node(child, set);
        }
    }

    // traverse up the tree
    open_node(node_to_visit->get_parent(), set);
}

int main(int argc, char* argv[]) {
    auto parent_node = std::make_shared<Node>();
    parent_node->set_parent(nullptr);
    parent_node->halo.mvir_ = 1.e10;
    parent_node->halo.set_id(0);
    parent_node->halo.set_parent_id(-1);

    auto child_nodeA = std::make_shared<Node>();
    auto child_nodeB = std::make_shared<Node>();
    auto grandchild_nodeA = std::make_shared<Node>();

    child_nodeA->halo.mvir_ = 5.e9;
    child_nodeA->halo.set_id(1);
    child_nodeA->halo.set_parent_id(0);
    std::cout << "Initial child_nodeA\n";
    child_nodeA->info();

    child_nodeB->halo.mvir_ = 1.e9;
    child_nodeB->halo.set_id(2);
    child_nodeB->halo.set_parent_id(0);
    std::cout << "Initial child_nodeB\n";
    child_nodeB->info();

    grandchild_nodeA->halo.mvir_ = 5.e8;
    grandchild_nodeA->halo.set_id(3);
    grandchild_nodeA->halo.set_parent_id(1);
    std::cout << "Initial grandchild_nodeA\n";
    grandchild_nodeA->info();

    parent_node->add_child(child_nodeA);
    parent_node->add_child(child_nodeB);
    child_nodeA->add_child(grandchild_nodeA);

    child_nodeA->info();
    child_nodeB->info();
    grandchild_nodeA->info();

    std::cout << "Traverse one branch.\n";

    auto node = parent_node;
    // traverse until we reach the end of the branch
    while (node != nullptr) {
        node->info();
        if (node->children_.size() > 0) {
            node = node->children_[0];
        }
        else {
            node = nullptr;
        }
    }

    node = parent_node;
    // stack of the the visited nodes
    auto set = std::make_shared<std::set<int64_t>>();
    set->insert(node->halo.get_id());

    // depth first search
    open_node(node, set);

    std::cout << "Visited IDs:\n";
    for (auto &set_value : *set) {
        std::cout << set_value << std::endl;
    }

    return 0;
}

