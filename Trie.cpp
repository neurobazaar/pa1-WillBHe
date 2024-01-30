#include "Trie.h"
#include <ctype.h>
#include <cassert>
#include <cstdio>
#include <cstring>

TrieNode::TrieNode()
	: count(0)
{
	for (int i = 0; i < 26; i++) 
		this->children[i] = nullptr;
}

TrieNode::~TrieNode()
{
	for (int i = 0; i < 26; i++)
	{
		delete this->children[i];
		this->children[i] = nullptr;
	}
}

Trie::Trie()
	: pHead(new TrieNode()),
	maxDepth(0)
{
}

Trie::~Trie()
{
	delete this->pHead;
}


//Really it finds and sets a word
//
bool Trie::Find(char *word, int wordLength)
{
	if (wordLength > this->maxDepth)
		this->maxDepth = wordLength;

	TrieNode* pCurr = this->pHead;
	int i = 0;
	while (i < wordLength)
	{
		int letter = tolower(word[i]) - 'a';
		if (letter >= 0 && letter <= 25)
		{
			if (!pCurr->children[letter])
				pCurr->children[letter] = new TrieNode();

			pCurr = pCurr->children[letter];

			word[i] = '0'; //This could be anything really its just to null out old data
			i++;
		}
	}

	pCurr->count++;

	return 1;
}

bool Trie::CreateDocumentWithData(FILE*& document)
{
	if (document == nullptr)
		return false;

	if (this->maxDepth > 50)
	{
		printf("Trie max depth is too great");
		return false;
	}
	const size_t size_tDepth = (size_t)maxDepth + 1;

	int* pTrieDepthMarker = new int[size_tDepth];
	char* buffer = new char[size_tDepth];
	TrieNode** pTrieStack = new TrieNode*[size_tDepth];


	memset(buffer, ' ', sizeof(buffer));
	size_t i = 0;
	while (i < size_tDepth)
	{
		buffer[i] = ' ';
		pTrieStack[i] = nullptr;
		pTrieDepthMarker[i] = 0;
		i++;
	}
	int iteratorColumn = 0;
	pTrieStack[iteratorColumn] = this->pHead;
	int iteratorRow = 0;

	buffer[size_tDepth - 1] = '\0';
	TrieNode* pCurr = pTrieStack[iteratorColumn];
	while (iteratorColumn >= 0)
	{
		while (iteratorRow < 26)
		{
			if (pCurr->children[iteratorRow] != nullptr)
			{
				//buffer[iteratorColumn] = iteratorRow + 'a';
				const char value = (char)iteratorRow + 'a';
				buffer[iteratorColumn] = value;
				pTrieDepthMarker[iteratorColumn] = iteratorRow + 1;


				//Is this a word essentially. If so add to document else move on
				if (pCurr->children[iteratorRow]->count > 0)
				{
					fprintf(document, "%.*s %d\n", iteratorColumn + 1 , buffer, pCurr->children[iteratorRow]->count);
					pTrieStack[iteratorColumn]->children[iteratorRow]->count = 0;
				}

				//Move up the stack
				iteratorColumn++;

				//Initialize the next spot on the triestack with the pointer to the next letter in the chain 
				pTrieStack[iteratorColumn] = pTrieStack[iteratorColumn - 1]->children[iteratorRow];
				pCurr = pTrieStack[iteratorColumn]; // For brevity

				//set iterator row to where we last left off
				//The next spot in depthmarker should always be 0
				iteratorRow = 0;
			}
			else
			{
				//Move along children if current child is null
				iteratorRow++;
			}
		}
		//Reset current level on the stack and DepthMarker
		pTrieDepthMarker[iteratorColumn] = 0;
		pTrieStack[iteratorColumn] = nullptr;

		//Pop down stack
		iteratorColumn--;

		//Get last place left off for previous and set data
		if (iteratorColumn >= 0)
		{
			pCurr = pTrieStack[iteratorColumn];
			iteratorRow = pTrieDepthMarker[iteratorColumn];
			buffer[iteratorColumn] = ' ';
		}
	}

	delete[] pTrieDepthMarker;
	delete[] buffer;
	delete[] pTrieStack;
	return true;
}

