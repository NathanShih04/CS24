#include "WordList.h"
#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>
using namespace std;

WordList::WordList(std::istream& stream) {
    string word;
    while(getline(stream, word)){
        bool isValid = true;
        for(char c : word){
            if(!islower(c) || !isalpha(c)){
                isValid = false;
                break;
            }
        }
        if(isValid){
            mWords.push_back(word);
        }
    }
}

float calculateScore(const vector<Point>& points, const string& word) {
    float totalScore = 0.0f;
    size_t length = points.size();
    for (size_t i = 0; i < length; ++i) {
        char c = word[i];
        const Point& wordPoint = QWERTY[c - 'a'];
        const Point& touchPoint = points[i];
        float dx = wordPoint.x - touchPoint.x;
        float dy = wordPoint.y - touchPoint.y;
        float distanceSquared = dx * dx + dy * dy;
        float score = 1.0f / (10 * distanceSquared + 1);
        totalScore += score;
    }

    return totalScore / length;
}

Heap WordList::correct(const vector<Point>& points, size_t maxcount, float cutoff) const {
    Heap heap(maxcount);
    size_t length = points.size();

    for(const string& word : mWords){
        if(word.length() != length){
            continue;
        }

        float score = calculateScore(points, word);
        if(score >= cutoff){
            if(heap.count() < maxcount){
                heap.push(word, score);
            } 
            else if(heap.top().score < score){
                heap.pushpop(word, score);
            }
        }
    }

    return heap;
}
