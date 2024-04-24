#include "Tree.h"

// Tree Function Implementations
Tree::Tree(){
    root = new Node;
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
    return 1 + countHelper(root->left) + countHelper(root->right);
}

size_t Tree::count() const{
    return countHelper(root);
}

// ---------------------------------------

bool Tree::contains(const std::string& s) const{
    return true;
}

size_t Tree::find(const std::string& s) const{
    return 1;
}

void Tree::insert(const std::string& s){

}

string Tree::lookup(size_t index) const{
    return " ";
}

void Tree::print() const{

}

void Tree::remove(size_t index){

}