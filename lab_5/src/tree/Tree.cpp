//
// Created by MaxPlays on 20/12/2024.
//

#include "Tree.h"

void deleteSubtree(TreeNode *node) {
    if (!node) return;

    deleteSubtree(node->left);
    deleteSubtree(node->right);

    if (node->available && node->pid > 0) {
        kill(node->pid, SIGTERM);
    }

    delete node;
}

TreeNode * deleteNodeById(TreeNode *root, int id) {
    if (!root) return nullptr;

    if (root->id == id) {
        deleteSubtree(root);
        return nullptr;
    }

    root->left = deleteNodeById(root->left, id);
    root->right = deleteNodeById(root->right, id);

    return root;
}
