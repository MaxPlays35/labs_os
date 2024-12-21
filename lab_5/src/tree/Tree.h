//
// Created by MaxPlays on 20/12/2024.
//
#pragma once

#include <iostream>

struct Node {
    int key;
    Node * left;
    Node * right;
    int size;

    explicit Node(int k) : key(k), left(nullptr), right(nullptr), size(1) {
    }
};

class Tree {
public:
    Tree();

    void insert(int key);

    void inorder_traversal();

private:
    Node * root;

    void insert(Node * current, int key);

    int get_size(Node * node);

    void inorder_traversal(Node * current);
};
