/*
Team Members: Carlos Navarro, Elyssa Esparza, Manuel Dixon
Course: CSC242 Intro to Programming Concepts
Assignment 5: Spell Checker (P8.2) & Monoalphabetic Cipher (P8.7)

Description:
This program provides two main functions.

1. Spell Checker:
   - Loads a dictionary file named dictionary.txt
   - Reads an input file named input.txt
   - Cleans punctuation and checks for words not found in the dictionary
   - Prints any words that may be misspelled

2. Monoalphabetic Keyword Cipher:
   - Encrypts or decrypts a file using a keyword-based substitution cipher
   - The keyword letters are used first with duplicate letters removed
   - The remaining alphabet letters are added in reverse order

Required Files:
dictionary.txt  - word list used for the spell checker
input.txt       - text file checked by the spell checker
cipherinput.txt - sample file used to test encryption or decryption

Note:
For this program to find the text files correctly, place the .txt files in the
main project folder and set Visual Studio's Working Directory to $(ProjectDir).
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cctype>

using namespace std;

/* ============================================================
   UTILITY FUNCTIONS
   ============================================================ */

   // Cleans a word (removes punctuation, converts to lowercase)
string cleanWord(const string& word)
{
    string cleaned;
    for (char c : word)
    {
        if (isalpha(c))
            cleaned += tolower(c);
    }
    return cleaned;
}

/* ============================================================
   SPELL CHECKER
   ============================================================ */

void runSpellChecker()
{
    unordered_set<string> dictionary;
    string word;

    ifstream dictFile("dictionary.txt");
    if (!dictFile)
    {
        cout << "Error: Could not open dictionary.txt\n";
        return;
    }

    // Load dictionary (FAST lookup structure)
    while (dictFile >> word)
    {
        string cleaned = cleanWord(word);
        if (!cleaned.empty())
            dictionary.insert(cleaned);
    }

    dictFile.close();

    cout << "Dictionary loaded: " << dictionary.size() << " words\n";

    ifstream inputFile("input.txt");
    if (!inputFile)
    {
        cout << "Error: Could not open input.txt\n";
        return;
    }

    cout << "\nWords NOT found in dictionary:\n";

    while (inputFile >> word)
    {
        string cleaned = cleanWord(word);

        if (!cleaned.empty() && dictionary.find(cleaned) == dictionary.end())
        {
            cout << cleaned << endl;
        }
    }

    inputFile.close();
}

/* ============================================================
   CIPHER
   ============================================================ */

   // Removes duplicates from keyword
string removeDuplicates(const string& keyword)
{
    string result;
    bool used[26] = { false };

    for (char c : keyword)
    {
        if (isalpha(c))
        {
            c = toupper(c);
            if (!used[c - 'A'])
            {
                result += c;
                used[c - 'A'] = true;
            }
        }
    }

    return result;
}

// Build cipher alphabet
string buildCipherAlphabet(const string& keyword)
{
    string cipher = removeDuplicates(keyword);
    bool used[26] = { false };

    for (char c : cipher)
        used[c - 'A'] = true;

    for (char c = 'Z'; c >= 'A'; c--)
    {
        if (!used[c - 'A'])
            cipher += c;
    }

    return cipher;
}

// Build mapping for encryption/decryption
unordered_map<char, char> buildMap(const string& cipher, bool encrypt)
{
    unordered_map<char, char> map;

    for (int i = 0; i < 26; i++)
    {
        char normal = 'A' + i;
        char coded = cipher[i];

        if (encrypt)
            map[normal] = coded;
        else
            map[coded] = normal;
    }

    return map;
}

// Process file using cipher map
void processCipher(const string& inputFile, const string& outputFile,
    const unordered_map<char, char>& map)
{
    ifstream in(inputFile);
    ofstream out(outputFile);

    if (!in)
    {
        cout << "Error: Could not open input file.\n";
        return;
    }

    if (!out)
    {
        cout << "Error: Could not create output file.\n";
        return;
    }

    char c;

    while (in.get(c))
    {
        if (isalpha(c))
        {
            bool lower = islower(c);
            char upper = toupper(c);

            if (map.find(upper) != map.end())
            {
                char mapped = map.at(upper);
                out << (lower ? char(tolower(mapped)) : mapped);
            }
            else
            {
                out << c; // fallback safety
            }
        }
        else
        {
            out << c;
        }
    }

    in.close();
    out.close();
}

void runCipher()
{
    char mode;
    string keyword, inputFile, outputFile;

    cout << "Enter E to encrypt or D to decrypt: ";
    cin >> mode;

    if (!(mode == 'E' || mode == 'e' || mode == 'D' || mode == 'd'))
    {
        cout << "Invalid mode.\n";
        return;
    }

    cout << "Enter keyword: ";
    cin >> keyword;

    if (keyword.empty())
    {
        cout << "Error: keyword required.\n";
        return;
    }

    cout << "Enter input file: ";
    cin >> inputFile;

    cout << "Enter output file: ";
    cin >> outputFile;

    string cipher = buildCipherAlphabet(keyword);
    bool encrypt = (mode == 'E' || mode == 'e');

    unordered_map<char, char> map = buildMap(cipher, encrypt);

    processCipher(inputFile, outputFile, map);

    cout << "Operation complete. Output saved to " << outputFile << endl;
}

/* ============================================================
   MAIN MENU
   ============================================================ */

int main()
{
    int choice;

    cout << "Assignment 5 Program\n";
    cout << "1. Spell Checker\n";
    cout << "2. Cipher\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        runSpellChecker();
        break;
    case 2:
        runCipher();
        break;
    default:
        cout << "Invalid choice.\n";
    }

    return 0;
}