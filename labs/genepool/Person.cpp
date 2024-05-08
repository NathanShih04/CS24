#include "Person.h"

// Person Member Functions

Person::Person(string name, Gender gender, Person* mother, Person* father) {
    pName = name;
    sex = gender;
    mom = mother;
    dad = father;
}

Person::~Person() {
    delete mom;
    delete dad;
}

// #########################################################################

const std::string& Person::name() const {
    return pName;
}

Gender Person::gender() const {
    return sex;
}

Person* Person::mother() {
    return mom;
}

Person* Person::father() {
    return dad;
}

// #########################################################################

std::set<Person*> Person::ancestors(PMod pmod) {
    set<Person*> allAncestors;

    if(pmod == PMod::ANY || pmod == PMod::MATERNAL){
        if(mom != nullptr){
            allAncestors.insert(mom);
        }
    }
    if(pmod == PMod::ANY || pmod == PMod::PATERNAL){
        if(dad != nullptr){
            allAncestors.insert(dad);
        }
    }

    // add mom parents and dad parents
    if(pmod == PMod::ANY || pmod == PMod::MATERNAL){
        if(mom != nullptr){
            set<Person*> maternalAncestors = mom->ancestors(PMod::MATERNAL);
            allAncestors.insert(maternalAncestors.begin(), maternalAncestors.end());
        }
    }
    if(pmod == PMod::ANY || pmod == PMod::PATERNAL){
        if(dad != nullptr){
            set<Person*> paternalAncestors = dad->ancestors(PMod::PATERNAL);
            allAncestors.insert(paternalAncestors.begin(), paternalAncestors.end());
        }
    }

    return allAncestors;
}

std::set<Person*> Person::aunts(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::brothers(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::children() {
    
}

std::set<Person*> Person::cousins(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::daughters() {
    
}

std::set<Person*> Person::descendants() {
    
}

std::set<Person*> Person::grandchildren() {
    
}

std::set<Person*> Person::granddaughters() {
    
}

std::set<Person*> Person::grandfathers(PMod pmod) {
    
}

std::set<Person*> Person::grandmothers(PMod pmod) {
    
}

std::set<Person*> Person::grandparents(PMod pmod) {
    
}

std::set<Person*> Person::grandsons() {
    
}

std::set<Person*> Person::nephews(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::nieces(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::parents(PMod pmod) {
    
}

std::set<Person*> Person::siblings(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::sisters(PMod pmod, SMod smod) {
    
}

std::set<Person*> Person::sons() {
    
}

std::set<Person*> Person::uncles(PMod pmod, SMod smod) {
    
}

// Other Member Functions
