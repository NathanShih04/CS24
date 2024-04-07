#include <iostream>
#include <string>
using namespace std;

string censor(int num, string message){
    int counter = 0;
    string result = "";

    for(unsigned int i = 0; i < message.length(); i++){
        if(message[i] != ' '){
            counter++;

            if((counter == num && i == message.length()) || (counter == num && message[i + 1] == ' ')){
                for(int j = num; j >= 0; j--){
                    result += message[i - j];
                }
                result += ' ';
            }
        }

        else{
            counter = 0;
        }
    }
    
    // remove end space
    if(result[result.length()] == ' '){
        result.pop_back();
    }

    return result;
}

int main(int argc, char *argv[]){
    string message;

    if(argc != 2){
        cout << "USAGE: censor [length]\n";
        return 1;
    }

    while(getline(cin, message)){
        cout << censor(stoi(argv[1]), message) << "\n";
    }

    return 0;
}