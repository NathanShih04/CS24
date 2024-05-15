#include "Person.h"
#include <set>

// Helper functions

void addSet(set<Person*>& originalSet, const set<Person*>& addedSet) {
    if(addedSet.size() == 0){
        return;
    }

    for(const auto& human : addedSet) {
        originalSet.insert(human);
    }
}
// void addSet(set<Person*> originalSet, set<Person*> addedSet) {
//     if(addedSet.size() == 0){
//         return;
//     }

//     for(Person* human : addedSet) {
//         originalSet.insert(human);
//     }
// }

// Person Member Functions

Person::Person(string name, Gender gender, Person* mother, Person* father) {
    pName = name;
    sex = gender;
    mom = mother;
    dad = father;
}

// Person::~Person() {
//     delete mom;
//     delete dad;
// }

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

std::set<Person*> Person::parents(PMod pmod) {
    set<Person*> parentSet;

    if(pmod == PMod::MATERNAL || pmod == PMod::ANY){
        if(mother()){
            parentSet.insert(mother());
        }
    }
    if(pmod == PMod::PATERNAL || pmod == PMod::ANY){
        if(father()){
            parentSet.insert(father());
        }
    }

    return parentSet;
}

std::set<Person*> Person::children() {
    return kids;
}

std::set<Person*> Person::sons() {
    set<Person*> sonSet;

    if(children().size() != 0){
        for(Person* child : children()){
            if(child->gender() == Gender::MALE){
                sonSet.insert(child);
            }
        }
    }

    return sonSet;
}

std::set<Person*> Person::daughters() {
    set<Person*> daughterSet;
    
    if(children().size() != 0){
        for(Person* child : children()){
            if(child->gender() == Gender::FEMALE){
                daughterSet.insert(child);
            }
        }
    }

    return daughterSet;
}

std::set<Person*> Person::grandparents(PMod pmod) {
    set<Person*> grandparentSet;

    if(pmod == PMod::MATERNAL || pmod == PMod::ANY){
        if(mother()){
            addSet(grandparentSet, mother()->parents(PMod::ANY));
        }
    }
    if(pmod == PMod::PATERNAL || pmod == PMod::ANY){
        if(father()){
            addSet(grandparentSet, father()->parents(PMod::ANY));
        }
    }

    return grandparentSet;
}

std::set<Person*> Person::grandfathers(PMod pmod) {
    set<Person*> grandfatherSet;

    if(pmod == PMod::MATERNAL && mother() != nullptr){
        addSet(grandfatherSet, mother()->parents(PMod::PATERNAL));
    }
    else if(pmod == PMod::PATERNAL && father() != nullptr){
        addSet(grandfatherSet, father()->parents(PMod::PATERNAL));
    }
    else{
        if(mother() != nullptr){
            addSet(grandfatherSet, mother()->parents(PMod::PATERNAL));
        }
        if(father() != nullptr){
            addSet(grandfatherSet, father()->parents(PMod::PATERNAL));

        }
    }

    return grandfatherSet;
}

std::set<Person*> Person::grandmothers(PMod pmod) {
    set<Person*> grandmotherSet;

    if(pmod == PMod::MATERNAL || pmod == PMod::ANY){
        if(mother()){
            addSet(grandmotherSet, mother()->parents(PMod::MATERNAL));
        }
    }
    if(pmod == PMod::PATERNAL || pmod == PMod::ANY){
        if(father()){
            addSet(grandmotherSet, father()->parents(PMod::MATERNAL));
        }
    }

    return grandmotherSet;
}

std::set<Person*> Person::grandchildren() {
    set<Person*> grandchildSet;

    if(children().size() != 0){
        set<Person*> childrenSet = children();
        for(Person* child : childrenSet) {
            set<Person*> grandchildrenSet = child->children();
            addSet(grandchildSet, grandchildrenSet);
        }
    }

    return grandchildSet;
}

std::set<Person*> Person::grandsons() {
    set<Person*> grandsonSet;

    if(grandchildren().size() != 0){
        for(Person* grandchild : grandchildren()){
            if(grandchild->gender() == Gender::MALE){
                grandsonSet.insert(grandchild);
            }
        }
    }

    return grandsonSet;
}

std::set<Person*> Person::granddaughters() {
    set<Person*> granddaughterSet;

    if(grandchildren().size() != 0){
        for(Person* grandchild : grandchildren()){
            if(grandchild->gender() == Gender::FEMALE){
                granddaughterSet.insert(grandchild);
            }
        }
    }

    return granddaughterSet;
}

std::set<Person*> Person::siblings(PMod pmod, SMod smod) {
     set<Person*> siblingSet;

    // pmods
    if(pmod == PMod::MATERNAL || pmod == PMod::ANY){
        if(mother()){
            addSet(siblingSet, mother()->children());
        }
    } 
    if(pmod == PMod::PATERNAL || pmod == PMod::ANY){
        if(father()){
            addSet(siblingSet, father()->children());
        }
    } 

    // smods
    // full siblings
    if(smod == SMod::FULL){
        for(Person* human : siblingSet){
            if((human->mother() != mother()) || (human->father() != father()) || (human->mother() == nullptr) || (human->father() == nullptr)){
                siblingSet.erase(human);
            }
        }
    }
    else if(smod == SMod::HALF){
        set<Person*> fullSiblings;
        addSet(fullSiblings, siblings(pmod, SMod::FULL));
        for(Person* human : fullSiblings){
            siblingSet.erase(human);
        }
    }

    siblingSet.erase(this);

    return siblingSet;
}

std::set<Person*> Person::brothers(PMod pmod, SMod smod) {
    set<Person*> brotherSet = siblings(pmod, smod);

    if(siblings(pmod, smod).size() != 0){
        for(Person* sibling : brotherSet){
            if(sibling->sex != Gender::MALE){
                brotherSet.erase(sibling);
            }
        }
    }

    return brotherSet;
}

std::set<Person*> Person::sisters(PMod pmod, SMod smod) {
    set<Person*> sisterSet = siblings(pmod, smod);

    if(siblings(pmod, smod).size() != 0){
        for(Person* sibling : sisterSet){
            if(sibling->sex != Gender::FEMALE){
                sisterSet.erase(sibling);
            }
        }
    }

    return sisterSet;
}

std::set<Person*> Person::uncles(PMod pmod, SMod smod) {
    set<Person*> uncleSet;

    if(parents(pmod).size() != 0){
        set<Person*> parentSet = parents(pmod);
        for(Person* parent : parentSet){
            set<Person*> parentBrothers = parent->brothers(PMod::ANY, smod);
            addSet(uncleSet, parentBrothers);
        }
    }

    return uncleSet;
}

std::set<Person*> Person::aunts(PMod pmod, SMod smod) {
    set<Person*> auntSet;

    if(parents(pmod).size() != 0){
        set<Person*> parentSet = parents(pmod);
        for(Person* parent : parentSet){
            set<Person*> parentSisters = parent->sisters(PMod::ANY, smod);
            addSet(auntSet, parentSisters);
        }
    }

    return auntSet;
}

std::set<Person*> Person::cousins(PMod pmod, SMod smod) {
    set<Person*> cousinSet;

    set<Person*> uncleOrAuntSet;
    if(pmod == PMod::MATERNAL){
        uncleOrAuntSet = uncles(PMod::MATERNAL, smod);
    } 
    else{
        uncleOrAuntSet = uncles(PMod::PATERNAL, smod);
    }

    for(Person* uncleOrAunt : uncleOrAuntSet){
        set<Person*> cousinChildren = uncleOrAunt->children();
        addSet(cousinSet, cousinChildren);
    }   

    return cousinSet;
}

set<Person*> Person::nephews(PMod pmod, SMod smod) {
    set<Person*> nephewSet;

    if(siblings(pmod, smod).size() != 0){
        set<Person*> siblingSet = siblings(pmod, smod);
        for(Person* sibling : siblingSet){
            set<Person*> siblingSons = sibling->sons();
            addSet(nephewSet, siblingSons);
        }
    }

    return nephewSet;
}

set<Person*> Person::nieces(PMod pmod, SMod smod) {
    set<Person*> nieceSet;

    if(siblings(pmod, smod).size() != 0){
        set<Person*> siblingSet = siblings(pmod, smod);
        for(Person* sibling : siblingSet){
            set<Person*> siblingDaughters = sibling->daughters();
            addSet(nieceSet, siblingDaughters);
        }
    }

    return nieceSet;
}

set<Person*> Person::descendants() {
    set<Person*> descendantSet;

    if(children().size() != 0){
    set<Person*> childrenSet = children();
        for(Person* child : childrenSet){
            descendantSet.insert(child);
            set<Person*> childDescendants = child->descendants();
            addSet(descendantSet, childDescendants);
        }
    }

    return descendantSet;
}

set<Person*> Person::ancestors(PMod pmod) {
    set<Person*> ancestorSet;

    set<Person*> parentSet = parents(pmod);

    addSet(ancestorSet, parentSet);
    for(Person* parent : parentSet){
        set<Person*> parentAncestors = parent->ancestors(pmod);
        addSet(ancestorSet, parentAncestors);
    }

    return ancestorSet;
}