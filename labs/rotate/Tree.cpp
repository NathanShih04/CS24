#include "Tree.h"
#include <stdexcept>
#include <iostream>
using namespace std;

// Helper functions

Node* indexHelper(Node* root, size_t index){
    size_t left_weight = 0;
    if (root->left != nullptr) {
        left_weight += root->left->weight;
    }

    if (left_weight == index){
        // std::cout << "8888" << std::endl;
        return root;
    } else if((root->left != nullptr) && (left_weight > index)){
        // std::cout << "9999" << std::endl;
        Node* ret = indexHelper(root->left, index);
        return ret;
    } else if (root->right != nullptr) {
        // std::cout << "10101010" << std::endl;
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
    if(root->left != nullptr){
        clearHelper(root->left);
        delete root->left;
        root->left = nullptr;
    }
    if(root->right != nullptr){
        clearHelper(root->right);
        delete root->right;
        root->right = nullptr;
    }

    delete root;
    root = nullptr;
}

void Tree::clear(){
    clearHelper(root);
}

// ---------------------------------------

Tree::~Tree(){
    if(root != nullptr){
        clearHelper(root);
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

bool containsHelper(Node* root, string s){
    if(root->word == s){
        return true;
    }
    else if(root->left != nullptr && s < root->word){
        containsHelper(root->left, s);
    }
    else if(root->right != nullptr && s > root->word){
        containsHelper(root->right, s);
    }
    
    return false;
}

bool Tree::contains(const std::string& s) const{
    return containsHelper(root, s);
}

// --------------------------------------- 
size_t findHelper(Node* node, const std::string& s, size_t skipped) {

    if (node == nullptr) {
        return 0xffffffffffffffff;
    }

    if (node->word == s) {
        // std::cout << "111" << std::endl;
        if (node->left != nullptr) {
            // std::cout << "222" << std::endl;
            return node->left->weight + skipped;
        }

        return skipped;
    } 

    if (node->word > s) {
        // std::cout << "333" << std::endl;

        if (node->left == nullptr) {
            return 0xffffffffffffffff;
        }
        return findHelper(node->left, s, skipped);
    }

    if (node->word < s) {
        // std::cout << "444" << std::endl;
        if (node->left != nullptr) {
            // std::cout << "555" << std::endl;
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

Node* insertHelper(Node* root, string s){

    if (root == nullptr) {
        root = new Node(s);
        // std::cout << root->weight << std::endl;
    }
    if(root->word > s) {
        // addLeft = true;
        if(root->left == nullptr){
            root->left = new Node(s);
            // std::cout << root->weight << std::endl;
        }
        root->left = insertHelper(root->left, s);
        
    }
    if(root->word < s) {
        // addRight = true;
        if(root->right == nullptr){
            root->right = new Node(s);
            // std::cout << root->weight << std::endl;
        }
        root-> right = insertHelper(root->right, s);
    }
    
    if ((root->left != nullptr) && (root->right != nullptr)){
        root->weight = root->left->weight + root->right->weight + 1;
    } 
    else if((root->right == nullptr) && (root->left != nullptr)) {
        root->weight = root->left->weight + 1;
    }
    else if((root->left == nullptr) && (root->right != nullptr)) {
        root->weight = root->right->weight + 1;
    }
    else {
        root->weight = 1;
    }

    return root;
}

void Tree::insert(const std::string& s){
    root = insertHelper(root, s);
}

// ---------------------------------------

string Tree::lookup(size_t index) const{
    std::string empty = "";
    Node* ret =  indexHelper(root, index);

    if(ret != nullptr){
        return ret->word;
    }

    return empty;
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
