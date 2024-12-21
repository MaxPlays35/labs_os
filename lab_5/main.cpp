//
// Created by MaxPlays on 20/12/2024.
//

#include "src/tree/Tree.h"

int main() {

    Tree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);

    std::cout << "Inorder Traversal: ";
    tree.inorder_traversal();
    return 0;
}
