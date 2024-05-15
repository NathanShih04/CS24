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
    if(mom != nullptr){
        return mom;
    }

    return nullptr;
}

Person* Person::father() {
    if(dad != nullptr){
        return dad;
    }
    
    return nullptr;
}

// #########################################################################

std::set<Person*> Person::parents(PMod pmod) {
    set<Person*> parentSet;

    if(pmod == PMod::MATERNAL && mom != nullptr){
        parentSet.insert(mom);
    }
    else if(pmod == PMod::PATERNAL && dad != nullptr){
        parentSet.insert(dad);
    }
    else{
        if(mom != nullptr){
            parentSet.insert(mom);
        }
        if(dad != nullptr){
            parentSet.insert(dad);
        }
    }

    return parentSet;
}

std::set<Person*> Person::children() {
    return kids;
}

std::set<Person*> Person::sons() {
    set<Person*> sonSet;

    if(children().size()!= 0){
        addSet(sonSet, children());
        for(Person* child : sonSet){
            if(child->sex != Gender::MALE){
                sonSet.erase(child);
            }
        }
    }

    return sonSet;
}

std::set<Person*> Person::daughters() {
    set<Person*> daughterSet;
    
    if(children().size() != 0){
        addSet(daughterSet, children());
        for(Person* child : daughterSet){
            if(child->sex != Gender::FEMALE){
                daughterSet.erase(child);
            }
        }
    }

    return daughterSet;
}

std::set<Person*> Person::grandparents(PMod pmod) {
    set<Person*> grandparentSet;

    if(pmod == PMod::MATERNAL && mom != nullptr){
        addSet(grandparentSet, mom->parents(PMod::ANY));
    }
    else if(pmod == PMod::PATERNAL && dad != nullptr){
        addSet(grandparentSet, dad->parents(PMod::ANY));
    }
    else{
        if(mom != nullptr){
            addSet(grandparentSet, mom->parents(PMod::ANY));
        }
        if(dad != nullptr){
            addSet(grandparentSet, dad->parents(PMod::ANY));
        }
    }

    return grandparentSet;
}

std::set<Person*> Person::grandfathers(PMod pmod) {
    set<Person*> grandfatherSet;

    if(pmod == PMod::MATERNAL && mom != nullptr){
        addSet(grandfatherSet, mom->parents(PMod::PATERNAL));
    }
    else if(pmod == PMod::PATERNAL && dad != nullptr){
        addSet(grandfatherSet, dad->parents(PMod::PATERNAL));
    }
    else{
        if(mom != nullptr){
            addSet(grandfatherSet, mom->parents(PMod::PATERNAL));
        }
        if(dad != nullptr){
            addSet(grandfatherSet, dad->parents(PMod::PATERNAL));

        }
    }

    return grandfatherSet;
}

std::set<Person*> Person::grandmothers(PMod pmod) {
    set<Person*> grandmotherSet;

    if(pmod == PMod::MATERNAL && mom != nullptr){
        addSet(grandmotherSet, mom->parents(PMod::MATERNAL));
    }
    else if(pmod == PMod::PATERNAL && dad != nullptr){
        addSet(grandmotherSet, dad->parents(PMod::MATERNAL));
    }
    else{
        if(mom != nullptr){
            addSet(grandmotherSet, mom->parents(PMod::MATERNAL));

        }
        if(dad != nullptr){
            addSet(grandmotherSet, dad->parents(PMod::MATERNAL));
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
        addSet(grandsonSet, grandchildren());
        for(Person* grandchild : grandsonSet){
            if(grandchild->sex != Gender::MALE){
                grandsonSet.erase(grandchild);
            }
        }
    }

    return grandsonSet;
}

std::set<Person*> Person::granddaughters() {
    set<Person*> granddaughterSet;

    if(grandchildren().size() != 0){
        addSet(granddaughterSet, grandchildren());
        for(Person* grandchild : granddaughterSet){
            if(grandchild->sex != Gender::FEMALE){
                granddaughterSet.erase(grandchild);
            }
        }
    }

    return granddaughterSet;
}

std::set<Person*> Person::siblings(PMod pmod, SMod smod) {
     set<Person*> siblingSet;

    // pmods
    if(pmod == PMod::MATERNAL && dad != nullptr){
        addSet(siblingSet, mom->children());
    } 
    else if (pmod == PMod::PATERNAL && mom != nullptr){
        addSet(siblingSet, dad->children());
    } 
    else{
        if(mom != nullptr){
            addSet(siblingSet, mom->children());
        }
        if(dad != nullptr){
            addSet(siblingSet, dad->children());
        }
    }

    // smods

    // full siblings
    if(smod == SMod::FULL){
        for(Person* human : siblingSet){
            if((human->mom != mom) || (human->dad != dad) || (human->mom == nullptr) || (human->dad == nullptr)){
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