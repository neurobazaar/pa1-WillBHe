#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include <cstring>


#include <dirent.h>
#include <sys/stat.h>
#include "unistd.h"
#include <bits/stdc++.h>

#include "Node.cpp"
void ISort(PointerNode** pHead)
{
	PointerNode* pSorted = *pHead;
	PointerNode* pUnSortedCurr = pSorted->pNext;

	pUnSortedCurr->pPrev = nullptr;
	pSorted->pNext = nullptr;

	while (pUnSortedCurr != nullptr) {
		PointerNode* pNext = pUnSortedCurr->pNext;

		if (pSorted->count <= pUnSortedCurr->count)
		{
			pUnSortedCurr->pNext = pSorted;
			pUnSortedCurr->pPrev = nullptr;
			pSorted->pPrev = pUnSortedCurr;
			pSorted = pUnSortedCurr;
		}
		else
		{
			PointerNode* pSortedCurr = pSorted;

			while (pSortedCurr->pNext && pSortedCurr->pNext->count > pUnSortedCurr->count)
			{
				pSortedCurr = pSortedCurr->pNext;
			}

			pUnSortedCurr->pNext = pSortedCurr->pNext;
			if (pSortedCurr->pNext)
				pSortedCurr->pNext->pPrev = pUnSortedCurr;
			pSortedCurr->pNext = pUnSortedCurr;
			pUnSortedCurr->pPrev = pSortedCurr;
		}
		pUnSortedCurr = pNext;
	}

	*pHead = pSorted;

}
int SortWordsByCount(FILE*& pInputFile, FILE*& pOutputFile)
{
	char buffer[100]; 
	char intBuffer[50];

	memset(intBuffer, '0', 50);
	if (fgets(buffer, sizeof(buffer), pInputFile) != nullptr)
	{
		size_t pCurr = strlen(buffer) - 1;
		int cutoffPoint = 48;

		//Get Integer - Fault Point
		while (buffer[pCurr] != ' ' && cutoffPoint > 0)
		{
			intBuffer[cutoffPoint] = buffer[pCurr];
			cutoffPoint--;
			pCurr--;
		}
		//memset(intBuffer, '0', 50);
		//intBuffer[49] = '\0';
	}
	PointerNode *pHead = new PointerNode(buffer, atoi(intBuffer));
	PointerNode *pCurrNode = pHead;
	int len = 0;

	//Go through N number of lines copy part of string
	// then create pointer PointerNode which stores the buffer data in DataNode 
	// then takes the count from the last part of the string 
	while (fgets(buffer, sizeof(buffer), pInputFile) != nullptr)
	{
		// Process each line here
		// We're getting ridiculous anyways
		memset(intBuffer, '0', 50);
		intBuffer[49] = '\0';

		size_t pCurr = strlen(buffer) - 1;
		int cutoffPoint = 48;
		while (buffer[pCurr] != ' ' && cutoffPoint > 0)
		{
			intBuffer[cutoffPoint] = buffer[pCurr];
			cutoffPoint--;
			pCurr--;
		}
		PointerNode *pTmp = new PointerNode(buffer, atoi(intBuffer), pCurrNode) ;
		pCurrNode->pNext = pTmp;
		pCurrNode = pCurrNode->pNext;
		len++;
	}

	ISort(&pHead);
	pCurrNode = pHead;
	while (pCurrNode)
	{
		PointerNode* const pTmp = pCurrNode->pNext;
		fprintf(pOutputFile, "%s", pCurrNode->pData->buffer);
		delete pCurrNode;
		pCurrNode = pTmp;
	}

	//fclose(pInputFile);
	//fclose(pOutputFile);

	return 0;
}

int main(int argc, char** argv)
{
        printf("Just checking whats going on here: %d\n", argc);

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
                snprintf(inpBuffer, strlen(inputPath) + strlen(entry->d_name) + 2, "%s/%s", inputPath, entry->d_name);
                snprintf(outBuffer, strlen(outputPath) + strlen(entry->d_name) + 2, "%s/%s", outputPath, entry->d_name);

                pInpDirCurrFolder = opendir(inpBuffer);
                pOutDirCurrFolder = opendir(outBuffer);
                if (!pOutDirCurrFolder)
                {
                        int check = mkdir(outBuffer, S_IRUSR | S_IWUSR | S_IXUSR);
                        printf("mkdir with %s returned: %d\n", outputPath, check);
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
                                        SortWordsByCount(pInputFile, pOutputFile);
                                        end = clock();

                                        stat(pInputFilePath, &st);
                                        printf("%.11s was: %ld  Cleaned in: %f\n", folderEntry->d_name, st.st_size,  (float(end - start))/ (float(CLOCKS_PER_SEC)));
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

