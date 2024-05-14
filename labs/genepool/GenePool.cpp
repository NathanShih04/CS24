#include "GenePool.h"
#include <sstream>
using namespace std;

// GenePool Member Functions

GenePool::GenePool(std::istream& stream){
    string message;

    while(getline(stream, message)){
        istringstream data(message);
        string token;
        string n, g, m, f;
    
        for(int i = 0; getline(data, token, '\t'); i++){
            if(i == 0){
                n = token;
            }
            else if(i == 1){
                g = token;
            }
            else if(i == 2){
                m = token;
            }
            else if(i == 3){
                f = token;
            }
        }

        // Gender stuff
        Gender gender = Gender::MALE;
        if(g == "female"){
            gender = Gender::FEMALE;
        } 

        // Mom pointer
        Person* mother = nullptr;
        if(m != "???"){
            mother = find(m);
        }

        // Dad pointer
        Person* father = nullptr;
        if(f != "???"){
            father = find(f);
        }

        Person* newPerson = new Person(n, gender, mother, father);
        if(mother != nullptr){
            mother->kids.insert(newPerson);
        }
        if(father != nullptr){
            father->kids.insert(newPerson);
        }
        
        famTree.insert({n, newPerson});
    }

}

GenePool::~GenePool() {
    for (auto& keyVal : famTree) {
        delete keyVal.second;
    }

    famTree.clear();
}

std::set<Person*> GenePool::everyone() const {
    set<Person*> famSet;

    for(auto& keyVal : famTree){
        famSet.insert(keyVal.second);
    }

    return famSet;
}

Person* GenePool::find(const std::string& name) const {
    if(famTree.size() != 0){
        for(auto& pair : famTree){
            if(pair.first == name){
                return pair.second;
            }
        }
    }

    return nullptr;
}