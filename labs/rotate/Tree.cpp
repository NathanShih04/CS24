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
        if(node->left != nullptr){
            return node->left->weight + skipped;
        }

        return skipped;
    } 

    if (node->word > s) {
        if (node->left == nullptr) {
            return 0xffffffffffffffff;
        }
        return findHelper(node->left, s, skipped);
    }

    if (node->word < s) {
        if (node->left != nullptr) {
            skipped += node->left->weight;
        }


        skipped += 1;

        if (node->right == nullptr) {
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

    if((output != 0xffffffffffffffff) || (output != 55555)){
        return false;
    }

    return true;

}

// --------------------------------------- 

Node* insertHelper(Node* root, string s){

    if(root == nullptr) {
        root = new Node(s);
    }

    if(root->word > s) {
        if(root->left == nullptr){
            root->left = new Node(s);
        }
        else{
            root->left = insertHelper(root->left, s);\
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
    else if(root->word == s){
        if(root->left == nullptr){
            root->left = new Node(s);
        }
        else{
            string holder = root->left->word;
            root->left->word = s;
            insertHelper(root->left, holder);
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
    std::string result = printNode(root);
    std::cout << result << std::endl;
}

// ---------------------------------------

void Tree::remove(size_t index){

}
