/*
Team Members: Carlos Navarro, Elyssa Esparza, Manuel Dixon
Course: CSC242 – Intro to Programming Concepts
Assignment 5 – Spell Checker (P8.2) & Monoalphabetic Cipher (P8.7)

Description:
This program provides two functionalities:

1. Spell Checker:
   - Loads a dictionary file (dictionary.txt)
   - Reads an input file (input.txt)
   - Cleans punctuation and checks for words not found in the dictionary

2. Monoalphabetic Keyword Cipher:
   - Encrypts or decrypts a file using a keyword-based substitution cipher
   - Keyword letters are used first (duplicates removed), followed by remaining
     alphabet letters in reverse order
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

/* ============================================================
   COMMON UTILITIES
   ============================================================ */

   // Removes punctuation and converts letters to lowercase
string cleanWord(const string& word) {
    string cleaned = "";
    for (char c : word) {
        if (isalpha(c)) {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

/* ============================================================
   SPELL CHECKER (P8.2)
   ============================================================ */

bool wordFound(const vector<string>& dictionary, const string& word) {
    for (const string& entry : dictionary) {
        if (entry == word) return true;
    }
    return false;
}

void runSpellChecker() {
    vector<string> dictionaryWords;
    string word;

    ifstream dictionaryFile("dictionary.txt");
    if (!dictionaryFile) {
        cout << "Error: Could not open dictionary.txt." << endl;
        return;
    }

    while (dictionaryFile >> word) {
        string cleaned = cleanWord(word);
        if (!cleaned.empty()) dictionaryWords.push_back(cleaned);
    }
    dictionaryFile.close();

    cout << "Dictionary loaded: " << dictionaryWords.size() << " words" << endl;

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cout << "Error: Could not open input.txt." << endl;
        return;
    }

    cout << "\nWords NOT found in dictionary:\n";

    while (inputFile >> word) {
        string cleaned = cleanWord(word);
        if (!cleaned.empty() && !wordFound(dictionaryWords, cleaned)) {
            cout << cleaned << endl;
        }
    }

    inputFile.close();
}

/* ============================================================
   CIPHER (P8.7)
   ============================================================ */

   // Removes duplicate letters and builds keyword portion
string removeDuplicateLetters(const string& keyword) {
    string result = "";
    bool used[26] = { false };

    for (char c : keyword) {
        if (isalpha(c)) {
            c = toupper(c);
            if (!used[c - 'A']) {
                result += c;
                used[c - 'A'] = true;
            }
        }
    }
    return result;
}

// Builds full cipher alphabet (keyword + remaining letters reversed)
string createCipherAlphabet(const string& keyword) {
    string cipher = removeDuplicateLetters(keyword);
    bool used[26] = { false };

    for (char c : cipher) used[c - 'A'] = true;

    for (char c = 'Z'; c >= 'A'; c--) {
        if (!used[c - 'A']) cipher += c;
    }

    return cipher;
}

// Builds mapping for encryption or decryption
unordered_map<char, char> buildMap(const string& cipher, bool encrypt) {
    unordered_map<char, char> map;
    for (int i = 0; i < 26; i++) {
        if (encrypt)
            map['A' + i] = cipher[i];
        else
            map[cipher[i]] = 'A' + i;
    }
    return map;
}

// Encrypts/decrypts file
void processCipherFile(const string& inputFile, const string& outputFile,
    const unordered_map<char, char>& map) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile) {
        cout << "Error: Could not open input file." << endl;
        return;
    }
    if (!outFile) {
        cout << "Error: Could not create output file." << endl;
        return;
    }

    char c;
    while (inFile.get(c)) {
        if (isalpha(c)) {
            bool lower = islower(c);
            char mapped = map.at(toupper(c));
            outFile << (lower ? tolower(mapped) : mapped);
        }
        else {
            outFile << c;
        }
    }

    inFile.close();
    outFile.close();
}

void runCipher() {
    char mode;
    string keyword, inputFile, outputFile;

    cout << "Enter E to encrypt or D to decrypt: ";
    cin >> mode;

    cout << "Enter keyword: ";
    cin >> keyword;

    cout << "Enter input file name: ";
    cin >> inputFile;

    cout << "Enter output file name: ";
    cin >> outputFile;

    string cipher = createCipherAlphabet(keyword);
    bool encrypt = (mode == 'E' || mode == 'e');

    unordered_map<char, char> map = buildMap(cipher, encrypt);

    processCipherFile(inputFile, outputFile, map);

    cout << "Cipher operation complete. Output saved to " << outputFile << endl;
}

/* ============================================================
   MAIN MENU
   ============================================================ */

int main() {
    int choice;

    cout << "Assignment 5 Program" << endl;
    cout << "1. Spell Checker" << endl;
    cout << "2. Cipher" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        runSpellChecker();
    }
    else if (choice == 2) {
        runCipher();
    }
    else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}
