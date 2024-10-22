#include <memory>
#include <vector>
#include <iostream>
#include "Halo.hpp"
#include "Node.hpp"

void Node::add_child(std::shared_ptr<Node> child) {
    children_.push_back(child);
    child->parent_ = shared_from_this();
    child->halo.set_parent_id(shared_from_this()->halo.get_id());
}

std::shared_ptr<Node> Node::get_parent(void) const {
    if (auto shared_parent = parent_.lock()) {
        return shared_parent;
    }
    else {
        return nullptr;
    }
}

void Node::set_parent(std::shared_ptr<Node> parent) {
    parent_ = parent;
}

void Node::info(void) const {
    std::cout << std::endl;
    std::cout << "-----------------------------\n";
    std::cout << "Parent information\n";
    std::cout << "-----------------------------\n";
    if (get_parent() != nullptr) {
        get_parent()->halo.info();
    }
    else {
        std::cout << "There is no parent node attached to this node.\n";
    }
    std::cout << "-----------------------------\n";

    std::cout << "Halo information\n";
    std::cout << "-----------------------------\n";
    Node::halo.info();
    std::cout << "-----------------------------\n";

    std::cout << "There are " << (int)children_.size() << " direct children of this halo.\n";
    std::cout << "-----------------------------\n" << std::endl;
}

