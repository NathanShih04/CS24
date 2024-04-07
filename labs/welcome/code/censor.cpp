#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;

string censor(int num, string message){
    vector<string> words;
    string result = "";
    string word;
    stringstream newMessage(message);
    unsigned int letters = num;

    if(newMessage)

    // Split the message by spaces and store the words in a vector
    while(newMessage >> word){

        words.push_back(word);
    }

    // for(unsigned int i = 0; i < message.length(); i++){
    //     if(message[i] != ' '){
    //         counter++;

    //         if((counter == num && i == message.length() - 1) || (counter == num && message[i + 1] == ' ')){
    //             for(int j = num; j >= 0; j--){
    //                 result += message[i - j];
    //             }
    //             result += ' ';
    //         }
    //     }

    //     else{
    //         counter = 0;
    //     }
    // }
    
    // remove end space
    for(unsigned int i = 0; i < words.size(); i++){
        if(words[i].size() != letters){
            result += words[i];
            result += ' ';
        }
    }

    result.pop_back();

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