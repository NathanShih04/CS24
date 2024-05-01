#include "Tree.h"
#include <stdexcept>
#include <iostream>
using namespace std;

// Helper functions

Node* indexHelper(Node* root, size_t index){
    size_t left_weight = 0;

    if(root != nullptr && root->left != nullptr){
        left_weight += root->left->weight;
    }

    if(left_weight == index){
        return root;
    } 
    else if((root != nullptr) && (root->left != nullptr) && (left_weight > index)){
        Node* ret = indexHelper(root->left, index);
        return ret;
    } 
    else if(root != nullptr && root->right != nullptr){
        Node* ret = indexHelper(root->right, index - left_weight - 1);
        return ret;
    }
    
    return nullptr;
}
// -----------------------------------------------------------

void calculateWeights(Node* root) {
    if (root == nullptr) {
        return;
    }

    calculateWeights(root->left);
    calculateWeights(root->right);

    if (root->left != nullptr && root->right != nullptr) {
        root->weight = root->left->weight + root->right->weight + 1;
    } else if (root->left != nullptr) {
        root->weight = root->left->weight + 1;
    } else if (root->right != nullptr) {
        root->weight = root->right->weight + 1;
    } else {
        root->weight = 1;
    }
}
// -----------------------------------------------------------

Node* copyTree(Node* root) {
    if(root == nullptr){
        return nullptr;
    }

    Node* newRoot = new Node(root->word, root->weight);

    // Recursively copy the left and right subtrees
    newRoot->left = copyTree(root->left);
    newRoot->right = copyTree(root->right);

    return newRoot;
}
// -----------------------------------------------------------

void rightRotate(Node* root){
    if(root == nullptr || root->right == nullptr){
        return;
    }
    Node* holder;
    holder = root->right;
    root->right = holder->left;
    holder->left = root;
}
// -----------------------------------------------------------

void leftRotate(Node* root){
    if(root == nullptr || root->left == nullptr){
        return;
    }
    Node* holder;
    holder = root->left;
    root->left = holder->right;
    holder->right = root;
}
// -----------------------------------------------------------
void balancer(Node* root){
    // Node* copiedRoot = copyTree(root);
    // int leftWeight;
    // int rightWeight;
    // int copiedLeft;
    // int copiedRight;
    // int oldDif;
    // int newDif;

    // if(root == nullptr){
    //     return;
    // }

    // if(root->left != nullptr){
    //     leftWeight = root->left->weight;
    // }
    // else{
    //     leftWeight = 0;
    // }

    // if(root->right != nullptr){
    //     rightWeight = root->right->weight;
    // }
    // else{
    //     rightWeight = 0;
    // }

    // oldDif = leftWeight - rightWeight;
    // if(oldDif < -1){
    //     rightRotate(copiedRoot);
    //     calculateWeights(copiedRoot);

    //     if(copiedRoot->left != nullptr){
    //         copiedLeft = copiedRoot->left->weight;
    //     }
    //     else{
    //         copiedLeft = 0;
    //     }

    //     if(copiedRoot->right != nullptr){
    //         copiedRight = copiedRoot->right->weight;
    //     }
    //     else{
    //         copiedRight = 0;
    //     }
        
    //     newDif = copiedLeft - copiedRight;
    //     if(newDif > oldDif){
    //         rightRotate(root);
    //     }

    // }

    // else if(oldDif > 1){
    //     leftRotate(copiedRoot);
    //     calculateWeights(copiedRoot);

    //     if(copiedRoot->left != nullptr){
    //         copiedLeft = copiedRoot->left->weight;
    //     }
    //     else{
    //         copiedLeft = 0;
    //     }

    //     if(copiedRoot->right != nullptr){
    //         copiedRight = copiedRoot->right->weight;
    //     }
    //     else{
    //         copiedRight = 0;
    //     }
        
    //     newDif = copiedLeft - copiedRight;
    //     if(newDif < oldDif){
    //         rightRotate(root);
    //     }
    // }
}

// ###########################################################

// Tree Function Implementations
Tree::Tree(){
    root = nullptr;
    size = 1;
}

// ---------------------------------------

void clearHelper(Node* root){
    if(root == nullptr){
        return;
    }
    if(root->left != nullptr) {
        clearHelper(root->left);
        delete root->left;
        root->left = nullptr;
    }
    if (root->right != nullptr) {
        clearHelper(root->right);
        delete root->right;
        root->right = nullptr;
    }
}

void Tree::clear(){
    clearHelper(root);
    delete root;
    root = nullptr;
}

// ---------------------------------------

Tree::~Tree(){
    if (root != nullptr) {
        clearHelper(root);
        delete root;
        root = nullptr;
    }

}

// ---------------------------------------

size_t countHelper(Node* root){
    if(root == nullptr) {
        return 0;
    }
    else{
        return root->weight;
    }

}

size_t Tree::count() const{
    return countHelper(root);
}

// ---------------------------------------

size_t findHelper(Node* node, const std::string& s, size_t skipped) {

    if(node == nullptr){
        return 0xffffffffffffffff;
    }

    if(node->word == s){
        if((node->left != nullptr) && (node->left->word != s)){
            return node->left->weight + skipped;
        }

        return skipped;
    } 

    if(node->word > s){
        if(node->left == nullptr){
            return 0xffffffffffffffff;
        }
        return findHelper(node->left, s, skipped);
    }

    if(node->word < s){
        if(node->left != nullptr){
            skipped += node->left->weight;
        }

        skipped += 1;

        if(node->right == nullptr){
            return 0xffffffffffffffff;
        }

        return findHelper(node->right, s, skipped);
    }

    return 55555;
}


size_t Tree::find(const std::string& s) const{
    return findHelper(root, s, 0);
}

// ---------------------------------------

bool Tree::contains(const std::string& s) const{
    size_t output = findHelper(root, s, 0);

    if((output == 0xffffffffffffffff) || (output == 55555)){
        return false;
    }

    return true;

}

// --------------------------------------- 

Node* insertHelper(Node* root, string s){

    if(root == nullptr) {
        root = new Node(s);
    }

    else if(root->word >= s) {
        if(root->left == nullptr){
            root->left = new Node(s);
        }
        else{
            root->left = insertHelper(root->left, s);
        }
        
    }
    else if(root->word < s) {
        if(root->right == nullptr){
            root->right = new Node(s);
        }
        else{
            root->right = insertHelper(root->right, s);
        }
    }
    
    if((root->left != nullptr) && (root->right != nullptr)){
        root->weight = root->left->weight + root->right->weight + 1;
    } 
    else if((root->right == nullptr) && (root->left != nullptr)){
        root->weight = root->left->weight + 1;
    }
    else if((root->left == nullptr) && (root->right != nullptr)){
        root->weight = root->right->weight + 1;
    }
    else{
        root->weight = 1;
    }

    // REBALANCING AND ROTATING
    balancer(root);

    return root;
}

void Tree::insert(const std::string& s){
    root = insertHelper(root, s);
}

// ---------------------------------------

string Tree::lookup(size_t index) const{
    string empty = "";
    Node* ret =  indexHelper(root, index);

    if(ret != nullptr){
        return ret->word;
    }

    throw std::out_of_range("Out of range.");
}

// ---------------------------------------

std::string printNode(Node* root) {
    if(root == nullptr){
        return "-";
    }

    if(root->word == ""){
        return "-";
    }

    if(root->left == nullptr && root->right == nullptr){
        return root->word;
    }

    std::string output = "(";
    if(root->left){
        output += printNode(root->left);
    }
    else {
        output += "-";
    }
    output+= " ";
    output += root->word;
    output += " ";

    if (root->right){
        output += printNode(root->right);
    }
    else{
        output += "-";
    }

    output += ")";

    return output;
}

void Tree::print() const{
    string result = printNode(root);
    cout << result << std::endl;
}

// ---------------------------------------

void removeHelper(Node* root, size_t index){
    if(root == nullptr){
        throw std::out_of_range("Index out of range.");
    }
    else if(indexHelper(root, index) == nullptr){
        throw std::out_of_range("Index out of range.");
    }
    // else if((indexHelper(root, index)->left == nullptr) && (indexHelper(root, index)->right == nullptr)){
    //     delete root;
    //     root = nullptr;
    // }
    // // one left child
    // else if((indexHelper(root, index)->left != nullptr) && (indexHelper(root, index)->right == nullptr)){

    // }
    // // one right child
    // else if((indexHelper(root, index)->right != nullptr) && (indexHelper(root, index)->left == nullptr)){

    // }

}

void Tree::remove(size_t index){
    removeHelper(root, index);
}

// Rotate checker old code
    // size_t dif;
    // size_t newDif;
    // size_t leftWeight;
    // size_t rightWeight;
    // size_t copiedLeftWeight;
    // size_t copiedRightWeight;

    // // Make a tree copy
    // Node* copiedRootLeft = copyTree(root);
    // Node* copiedRootRight = copyTree(root);

    // if(root == nullptr){
    //     return;
    // }

    // if(root->left == nullptr){
    //     leftWeight = 0;
    // }
    // else{
    //     leftWeight = root->left->weight;
    // }
    // if(root->right == nullptr){
    //     rightWeight = 0;
    // }
    // else{
    //     rightWeight = root->right->weight;
    // }
    

    // // Check to see if rotating helps left
    // if(leftWeight > rightWeight){
    //     dif = leftWeight - rightWeight;
    //     leftRotate(copiedRootLeft);
    //     calculateWeights(copiedRootLeft);
    //     newDif = copiedRootLeft->left->weight - copiedRootLeft->right->weight;
        
    //     if(newDif < dif){
    //         leftRotate(root);
    //     }
    // }
    // // Check to see if rotating helps right
    // else if(rightWeight > leftWeight){
    //     dif = rightWeight - leftWeight;
    //     leftRotate(copiedRootRight);
    //     calculateWeights(copiedRootRight);
    //     newDif = copiedRootRight->right->weight - copiedRootRight->left->weight;
        
    //     if(newDif < dif){
    //         rightRotate(root);
    //     }
    // }