#include "GenePool.h"
#include <sstream>
using namespace std;

// GenePool Member Functions

GenePool::GenePool(std::istream& stream){
    string message;

    while(getline(stream, message)){
        
        if(message.empty() || message[0] == '#'){
            continue;
        }

        istringstream data(message);
        string token;
        string n, g, m, f;
        Gender gend = Gender::MALE;
        Person* ma = nullptr;
        Person* pa = nullptr;

        for(int i = 0; getline(data, token, '\t'); i++){
            if(i == 0){
                n = token;
            }
            if(i == 1){
                g = token;
                if(g == "female"){
                    gend = Gender::FEMALE;
                } 
            }
            if(i == 2){
                m = token;
                if(m != "???"){
                    ma = find(m);
                }
            }
            if(i == 3){
                f = token;
                if(f != "???"){
                    pa = find(f);
                }
            }
        }

        Person* member = new Person(n, gend, ma, pa);
        if(ma != nullptr){
            ma->kids.insert(member);
        }
        if(pa != nullptr){
            pa->kids.insert(member);
        }
        
        famTree.insert({n, member});
    }

}

GenePool::~GenePool() {
    for(auto it = famTree.begin(); it != famTree.end(); it++){
        delete it->second;
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