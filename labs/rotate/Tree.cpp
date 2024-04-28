#include "Tree.h"
#include <stdexcept>
#include <iostream>
using namespace std;

// Helper functions

Node* indexHelper(Node* root, size_t index){

    if(root->left != nullptr && root->left->weight == index){
        return root;
    }
    else if(root->left != nullptr && root->left->weight > index){
        indexHelper(root->left, index - 1);
    }
    else if(root->right->weight < index){
        indexHelper(root->right, index - root->left->weight - 1);
    }
    
    return nullptr;
}

// Tree Function Implementations
Tree::Tree(){
    root = nullptr;
    size = 1;
}

// ---------------------------------------

void destructorHelper(Node* root){
    if(root != nullptr){
        destructorHelper(root->left);
        destructorHelper(root->right);
        delete root;
    }
}

Tree::~Tree(){
    destructorHelper(root);
}

// ---------------------------------------

void clearHelper(Node* root){
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
    if (node->word == s) {
        if (node->left != nullptr) {
            return node->left->weight + skipped;
        }

        return skipped;
    } 

    if (node->word > s) {
        return findHelper(node->left, s, skipped);
    }

    if (node->word < s) {
        if (node->left != nullptr) {
            skipped += node->left->weight;
        }

        skipped += 1;

        return findHelper(node->right, s, skipped);
    }

    return 0;
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
    return indexHelper(root, index)->word;
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
