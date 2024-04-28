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
    else if(root->right != nullptr && root->right->weight < index){
        indexHelper(root->right, index - root->left->weight - 1);
    }
    
    return nullptr;
}

void balanceHelper(Node* root){

}

// Tree Function Implementations
Tree::Tree(){
    root = nullptr;
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

size_t Tree::find(const std::string& s) const{
    return 1;
}

// ---------------------------------------

Node* insertHelper(Node* root, string s){
    if(root == nullptr){
        root = new Node;
    }

    if(root->word > s){
        if(root->left == nullptr){
            root->left = new Node(s);
            root->weight += root->left->weight;
        }
        insertHelper(root->left, s);
    }

    if(root->word < s){
        if(root->right == nullptr){
            root->right = new Node(s);
            root->weight += root->right->weight;
        }
        insertHelper(root->right, s);
    }

    return root;
}

void Tree::insert(const std::string& s){
    insertHelper(root, s);
}

// ---------------------------------------

string Tree::lookup(size_t index) const{
    return indexHelper(root, index)->word;
}

// ---------------------------------------

string printHelper(Node* root){

    string final = "";

    if(root == nullptr){
        final += "-";
        return final;
    }

    if(root->word == ""){
        final += "-";
        return final;
    }

    if(root->left == nullptr && root->right == nullptr){
        final += root->word;
    }

    final += "(";
    printHelper(root->left);
    final += " ";
    final += root->word;
    final += " ";
    printHelper(root->right);
    final += ")";

    return final;
}

void Tree::print() const{
    cout << printHelper(root);
}

// ---------------------------------------

void Tree::remove(size_t index){

}