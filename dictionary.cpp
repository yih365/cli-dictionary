#include <bits/stdc++.h>
#include "dictionary.hpp"
using namespace std;

// node structure
class node {
public:
	pair<string, string> defPair;
	node *next;
};

// size of hash table
const unsigned int N = 10400;
long dictionarySize = 0;

node *table[N];
node *head = NULL;

// returns size of the dictionary
long getDictionarySize() {
	return dictionarySize;
}

// load words and definition into hashtable
bool loadWords(string filename) {
	fstream fin;
	fin.open(filename, ios::in);
	
	string line = "";
	while(!fin.eof()) {
		getline(fin, line);

		if (line == "") continue;

		// split word from meaning
		int index = 0;
		for (unsigned int i = 0; i < line.length(); i ++) {
			char c = line[i];
			if (c == '(') {
				index = i-1;
				break;
			}
		}
		// possible error
		if (index <= 1) continue;
		
		int startIndex = 0;
		while (!isalpha(line[startIndex])) startIndex++;
		
		int defIndex = index+1;
		while(index > 0 && line[index] == ' ') index--;
		string word = line.substr(startIndex, index+1-startIndex);
		word = stringToLower(word);
		string meaning = line.substr(defIndex);

		pair<string, string> wordPair (word, meaning);
		unsigned int wordHash = hashWord(word);

		// create node for word
		node *n = new node();

		n->defPair = wordPair;
		n->next = table[wordHash];
		table[wordHash] = n;

		dictionarySize ++;
	}

	fin.close();
	return true;
}

// turn word to lowercase
string stringToLower (string word) {
	for (unsigned int i = 0; i < word.length(); i ++) {
		if (isalpha(word[i])) word[i] = tolower(word[i]);
	}
	return word;
}

// hashes word to number
unsigned int hashWord(const string word) {
	long hashValue = 0;
	for (char c: word) {
		int num = (int) c;
		hashValue += num;
	}
	unsigned int index = hashValue%N;
	return index;	
}

// search for the definition of a word
string searchForWord(const string word) {
	string searchWord = stringToLower(word);
	unsigned int index = hashWord(searchWord);
	string returnText = "";
	
	node *n = table[index];
	while(n != NULL) {
		pair<string, string> currentPair = n->defPair;
		if (currentPair.first == searchWord) {
			returnText += currentPair.first + " " + currentPair.second;
			returnText += '\n';
		} 
		n = n->next;
	}
	if (returnText == "") {
		returnText = "There is no match.";
		returnText += '\n';
	}
	return returnText;
}

// free memory that is storing the dictionary
bool unloadDictionary() {
	for (unsigned int i = 0; i < N; i ++) {
		node *tmp = table[i];
		while(tmp != NULL) {
			node *n = tmp;
			tmp = tmp->next;
			delete n;
		}
	}
	return true;
}

