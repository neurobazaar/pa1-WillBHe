#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "Trie.h"
#include <cstring>
#include "Node.h"

#include <dirent.h>
#include <sys/stat.h>
#include "unistd.h"
#include <bits/stdc++.h>

// ---------------------------
// Helper Functions
// ---------------------------

bool isUsableDelimiter(int c)
{
	switch (c)
	{
	case ' ':
	case '\\':
	case '\t':
	case '\n':
		return true;
	default:
		return false;
	}
}

int CleanDataSet(FILE*& pInputFile, FILE*& pOutputFile)
{
	int res = 0;
	if (pInputFile && pOutputFile)
	{
		int c, lastDelimiter = 0;
		while ((c = fgetc(pInputFile)) != EOF)
		{
			//printf("%c", c);
			if (isUsableDelimiter(c))
			{
				lastDelimiter = c;
			}
			else if (isalnum(c)) // Check if vars are alphanumeric
			{
				if (lastDelimiter != 0)
				{
					fputc(lastDelimiter, pOutputFile);
					lastDelimiter = 0;
				}
				fputc(c, pOutputFile);
			}
		}
		res = 1;
	}
	return res;
}

int main(int argc, char** argv)
{
	printf("Just checking whats going on here file size: %d\n", argc);

	if (argc != 3)
	{
		printf("Not enough arguments\n");
		return 1;
	}
	char cwd[256];
	char inputPath[256];
        char outputPath[256];

	if(getcwd(cwd, sizeof(cwd)))
	{
		printf("cwd: %s\n", cwd);
		snprintf(inputPath, strlen(argv[1]) + strlen(cwd) + 2, "%s%s", cwd, argv[1]);
		snprintf(outputPath, strlen(argv[2]) + strlen(cwd) + 2, "%s%s", cwd, argv[2]);
	}

	char inpBuffer[256];
	char outBuffer[256];

	printf("in : %s\n",inputPath);
	printf("out : %s\n",outputPath);
	memset(inpBuffer, ' ', sizeof(inpBuffer));
	memset(outBuffer, ' ', sizeof(outBuffer));

DIR* pInputDir = nullptr, * pOutputDir = nullptr,
	* pInpDirCurrFolder = nullptr, * pOutDirCurrFolder = nullptr;

FILE* pInputFile = nullptr, * pOutputFile = nullptr;
struct stat st;


pInputDir = opendir(inputPath); 
if (pInputDir != nullptr)
{
	//If no outputDir then we make one
	if (!(pOutputDir = opendir(outputPath)))
	{
		//CreateDirectoryA(LPCSTR(outputPath), nullptr);
		int check = mkdir(outputPath, S_IRUSR | S_IWUSR | S_IXUSR);
		printf("mkdir with %s returned: %d", outputPath, check);
	}

	char pOutputTimePath[256];
	//Sorry im getting so sloppy right here
	snprintf(pOutputTimePath, strlen(outputPath) + 20, "%s/%s", outputPath, "OperationTimes.txt"); 
	FILE *pOutputTimesFile = fopen(pOutputTimePath, "w");

	if(pOutputTimesFile == nullptr)
	{
		printf("Couldn't Create Timing file");
		return 1;
	}

	dirent* entry = nullptr;
	while ((entry = readdir(pInputDir)) != nullptr)
	{
		if (entry->d_name[0] == '.')
		{
			continue;
		}

		printf("-------------\n%s\n-------------\n", entry->d_name);
		//fprintf(pOutputTimesFile, "\n%s\n", entry->d_name);
		snprintf(inpBuffer, strlen(inputPath) + strlen(entry->d_name) + 2, "%s/%s", inputPath, entry->d_name);
		snprintf(outBuffer, strlen(outputPath) + strlen(entry->d_name) + 2, "%s/%s", outputPath, entry->d_name);

		pInpDirCurrFolder = opendir(inpBuffer);
		pOutDirCurrFolder = opendir(outBuffer);
		if (!pOutDirCurrFolder)
		{
			int check = mkdir(outBuffer, S_IRUSR | S_IWUSR | S_IXUSR); 
			printf("mkdir with %s\%s returned: %d\n", outputPath, outBuffer, check);
		}

		// If theres no input just go next
		if (pInpDirCurrFolder)
		{
			//If theres no output we can make one
			if (!pOutDirCurrFolder)
			{
				mkdir(outBuffer, S_IRUSR | S_IWUSR | S_IXUSR);
			}
			dirent* folderEntry = nullptr;
			while ((folderEntry = readdir(pInpDirCurrFolder)) != nullptr)
			{
				if (folderEntry->d_type == DT_REG)
				{

					//Initialze then fill string f the input file path 
					size_t size = strlen(inpBuffer) + strlen(folderEntry->d_name) + 2;
					char* pInputFilePath = new char[size];
					memset(pInputFilePath, ' ', size);
					snprintf(pInputFilePath, size, "%s/%s", inpBuffer, folderEntry->d_name);

					//Initialze then fill string for the output file path 
					size = strlen(outBuffer) + strlen(folderEntry->d_name) + 2;
					char* pOutputFilePath = new char[size];
					memset(pOutputFilePath, ' ', size);
					snprintf(pOutputFilePath, size, "%s/%s", outBuffer, folderEntry->d_name);

					if ((pInputFile = fopen(pInputFilePath, "r")) == nullptr)
					{
						printf("Input file: %s coult not be opened\n", pInputFilePath);
						break;
					}
					if ((pOutputFile = fopen(pOutputFilePath, "w")) == nullptr)
					{
						printf("Output file: %s coult not be opened\n", pOutputFilePath);
						break;
					}

					clock_t start, end;
					start =  clock();
					CleanDataSet(pInputFile, pOutputFile);
					end = clock();

					stat(pInputFilePath, &st);
					fprintf(pOutputTimesFile,"%.11s was: %ld  Cleaned in: %f\n", folderEntry->d_name, st.st_size,  (float(end - start))/ (float(CLOCKS_PER_SEC)));
					// Ideally for performance I would either allocate this ahead of time/never try to new it
					delete[] pInputFilePath;
					delete[] pOutputFilePath;
					fclose(pInputFile);
					fclose(pOutputFile);
				}
			}
		}
		else
		{
			printf("pInputFolder wasn't there\n");
		}
		closedir(pInpDirCurrFolder);
		closedir(pOutDirCurrFolder);
	}
}
else
{
	printf("pInputDir wasn't there\n");
}
closedir(pInputDir);
closedir(pOutputDir);

printf("This is just to make sure it closed\n");
return 0;
}
