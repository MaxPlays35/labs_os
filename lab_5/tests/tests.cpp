//
// Created by MaxPlays on 20/12/2024.
//

#include <gtest/gtest.h>

#include "tree/Tree.h"


TEST(TreeNodeTest, DeleteSubtree) {
    TreeNode* root = new TreeNode(1, 12345, 8080);
    root->left = new TreeNode(2, 12346, 8081);
    root->right = new TreeNode(3, 12347, 8082);
    root->left->left = new TreeNode(4, 12348, 8083);

    deleteSubtree(root);
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->right, nullptr);
}

TEST(TreeNodeTest, DeleteNodeById) {
    TreeNode* root = new TreeNode(1, 12345, 8080);
    root->left = new TreeNode(2, 12346, 8081);
    root->right = new TreeNode(3, 12347, 8082);
    root->left->left = new TreeNode(4, 12348, 8083);

    root = deleteNodeById(root, 2);

    // After deletion, left subtree should be null
    EXPECT_EQ(root->left, nullptr);

    // Right subtree and root should remain unchanged
    EXPECT_NE(root, nullptr);
    EXPECT_EQ(root->id, 1);
    EXPECT_NE(root->right, nullptr);
    EXPECT_EQ(root->right->id, 3);

    deleteSubtree(root);
}

// Test case: Delete non-existing node
TEST(TreeNodeTest, DeleteNonExistingNode) {
    TreeNode* root = new TreeNode(1, 12345, 8080);
    root->left = new TreeNode(2, 12346, 8081);
    root->right = new TreeNode(3, 12347, 8082);

    TreeNode* result = deleteNodeById(root, 999);

    // Ensure tree structure remains unchanged
    EXPECT_EQ(result, root);
    EXPECT_NE(root->left, nullptr);
    EXPECT_NE(root->right, nullptr);

    deleteSubtree(root);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
