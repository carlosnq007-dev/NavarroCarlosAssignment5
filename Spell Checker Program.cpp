/*
Team Members: Carlos Navarro, Elyssa Esparza, Manuel Dixon
Assignment 5: Spell Checker
Description: Checks spelling of words using dictionary file
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Member 1: Load dictionary
vector<string> loadDictionary(string filename) {
    vector<string> words;
    ifstream file(filename);
    string word;

    while (file >> word) {
        words.push_back(word);
    }

    return words;
}

// Member 2: Check if word exists
bool wordExists(const vector<string>& dict, string word) {
    return find(dict.begin(), dict.end(), word) != dict.end();
}

// Member 3: Process file
void checkSpelling(string filename, const vector<string>& dict) {
    ifstream file(filename);
    string word;

    while (file >> word) {
        if (!wordExists(dict, word)) {
            cout << word << endl;
        }
    }
}

int main() {
    vector<string> dictionary = loadDictionary("dictionary.txt");
    checkSpelling("input.txt", dictionary);
    return 0;
}