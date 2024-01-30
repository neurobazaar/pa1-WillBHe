#pragma once
#include "ctype.h"
#include "stdio.h"
const int ALPHABET_SIZE = 26;

class TrieNode
{
public:
	TrieNode* children[ALPHABET_SIZE];
	int count;

	TrieNode();
	~TrieNode();
};

class Trie
{
public:
	TrieNode *pHead;
	int maxDepth; //This can be used to define maximum

	Trie();
	~Trie();

	bool Find(char *word, int wordLength);
	bool CreateDocumentWithData(FILE*& document);
private:

};
