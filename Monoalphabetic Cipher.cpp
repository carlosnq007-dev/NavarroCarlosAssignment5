/*
Team Members: Carlos Navarro, Elyssa Esparza, Manuel Dixon
Assignment: 5 - P8.7 Cipher
Description: Encrypts/Decrypts using keyword cipher
*/

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

// Member 1: Generate cipher alphabet
string generateCipher(string keyword) {
    string cipher = "";
    bool used[26] = { false };

    // Add unique letters from keyword
    for (char c : keyword) {
        c = toupper(c);
        if (!used[c - 'A']) {
            cipher += c;
            used[c - 'A'] = true;
        }
    }

    // Add remaining letters in reverse
    for (char c = 'Z'; c >= 'A'; c--) {
        if (!used[c - 'A']) {
            cipher += c;
        }
    }

    return cipher;
}

// Member 2: Build mapping
unordered_map<char, char> buildMap(string cipher, bool encrypt) {
    unordered_map<char, char> map;

    for (int i = 0; i < 26; i++) {
        if (encrypt)
            map['A' + i] = cipher[i];
        else
            map[cipher[i]] = 'A' + i;
    }

    return map;
}

// Member 3: Process file
void processFile(string inputFile, string outputFile, unordered_map<char, char> map) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    char c;

    while (inFile.get(c)) {
        if (isalpha(c)) {
            char upper = toupper(c);
            char mapped = map[upper];
            outFile << mapped;
        }
        else {
            outFile << c;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "Usage: crypt -d/-e -kKEY input output" << endl;
        return 1;
    }

    bool decrypt = string(argv[1]) == "-d";
    string keyword = string(argv[2]).substr(2);
    string inputFile = argv[3];
    string outputFile = argv[4];

    string cipher = generateCipher(keyword);
    auto map = buildMap(cipher, !decrypt);

    processFile(inputFile, outputFile, map);

    return 0;
}