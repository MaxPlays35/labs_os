//
// Created by MaxPlays on 20/12/2024.
//

#include "Tree.h"

Tree::Tree(): root(nullptr) {
}

void Tree::insert(int key) {
    if (root == nullptr) {
        root = new Node(key);
    } else {
        insert(root, key);
    }
}

void Tree::inorder_traversal() {
    inorder_traversal(root);
    std::cout << std::endl;
}

void Tree::insert(Node * current, int key) {
    if (current->left == nullptr) {
        current->left = new Node(key);
    } else if (current->right == nullptr) {
        current->right = new Node(key);
    } else {
        if (get_size(current->left) <= get_size(current->right)) {
            insert(current->left, key);
        } else {
            insert(current->right, key);
        }
    }
    current->size = 1 + get_size(current->left) + get_size(current->right);
}

int Tree::get_size(Node * node) {
    return node ? node->size : 0;
}

void Tree::inorder_traversal(Node * current) {
    if (current) {
        inorder_traversal(current->left);
        std::cout << current->key << " ";
        inorder_traversal(current->right);
    }
}
