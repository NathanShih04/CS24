#include "GenePool.h"

// GenePool Member Functions

GenePool::GenePool(std::istream& stream) {

}

GenePool::~GenePool() {

}

std::set<Person*> GenePool::everyone() const {
    return famTree;
}

Person* GenePool::find(const std::string& name) const {
    
}