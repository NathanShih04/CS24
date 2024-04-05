#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]){

    if(argc != 2){
        cout << "USAGE: censor [length]\n";
        return 1;
    }

    cout << stoi(argv[1]);
}