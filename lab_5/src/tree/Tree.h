//
// Created by MaxPlays on 20/12/2024.
//
#pragma once

#include <iostream>
#include <signal.h>

struct TreeNode {
    int id;
    pid_t pid;
    int port;
    TreeNode* left;
    TreeNode* right;
    bool available;

    TreeNode(int node_id, pid_t process_id, int node_port)
        : id(node_id), pid(process_id), port(node_port), left(nullptr), right(nullptr), available(true) {}
};

void deleteSubtree(TreeNode* node);

TreeNode* deleteNodeById(TreeNode* root, int id);
