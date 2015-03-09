// CodeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cstdio"
#include "cstdint"
#include "cinttypes"
#include "limits"
#include "iostream"
#include "string"
#include "cstring"
#include "fstream"
#include "list"
#include "algorithm"
#include "cstddef";
#include "windows.h";
#include <Wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wincrypt.h>
#include <ctime>
#include <iomanip>


std::string anagram = "poultry outwits ants";


char* HashMD5(char* data, DWORD *result)
{
	DWORD dwStatus = 0;
	DWORD cbHash = 16;
	int i = 0;
	HCRYPTPROV cryptProv;
	HCRYPTHASH cryptHash;
	BYTE hash[16];
	char *hex = "0123456789abcdef";
	char *strHash;
	strHash = (char*)malloc(500);
	memset(strHash, '\0', 500);
	if (!CryptAcquireContext(&cryptProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		*result = dwStatus;
		return NULL;
	}
	if (!CryptCreateHash(cryptProv, CALG_MD5, 0, 0, &cryptHash))
	{
		dwStatus = GetLastError();
		printf("CryptCreateHash failed: %d\n", dwStatus);
		CryptReleaseContext(cryptProv, 0);
		*result = dwStatus;
		return NULL;
	}
	if (!CryptHashData(cryptHash, (BYTE*)data, strlen(data), 0))
	{
		dwStatus = GetLastError();
		printf("CryptHashData failed: %d\n", dwStatus);
		CryptReleaseContext(cryptProv, 0);
		CryptDestroyHash(cryptHash);
		*result = dwStatus;
		return NULL;
	}
	if (!CryptGetHashParam(cryptHash, HP_HASHVAL, hash, &cbHash, 0))
	{
		dwStatus = GetLastError();
		printf("CryptGetHashParam failed: %d\n", dwStatus);
		CryptReleaseContext(cryptProv, 0);
		CryptDestroyHash(cryptHash);
		*result = dwStatus;
		return NULL;
	}
	for (i = 0; i < cbHash; i++)
	{
		strHash[i * 2] = hex[hash[i] >> 4];
		strHash[(i * 2) + 1] = hex[hash[i] & 0xF];
	}
	CryptReleaseContext(cryptProv, 0);
	CryptDestroyHash(cryptHash);
	return strHash;
}

bool stringMatchHash(std::string phase)
{
	char* hash = "4624d200580677270a54ccff86b9610e";
	//phase.erase(remove_if(phase.begin(), phase.end(), isspace), phase.end());
	//sort(phase.begin(), phase.end());
	if (phase.length() == anagram.length())
	{
		DWORD* result = 0;
		char* hs = HashMD5((char*)phase.c_str(), result);
		if (strcmp(hash, hs) == 0)
		{
			return true;
		}
	}
	return false;
}

std::string findPhase(std::list<std::string> wordList)
{
	std::string phase;

	for each (std::string word1 in wordList)
	{
		std::list<std::string> ele2 = wordList;
		ele2.remove(word1);
		for each (std::string word2 in ele2)
		{
			std::list<std::string> ele3 = ele2;
			ele3.remove(word1);
			for each (std::string word3 in ele3)
			{
				std::string p = word1 + " " + word2 + " " + word3;
				bool b = stringMatchHash(p);
				if (b)
				{
					phase = p;
					return phase;
				}
			}
		}
	}
	return phase;
}
int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

	//anagram.erase(remove_if(anagram.begin(), anagram.end(), isspace), anagram.end());
	//sort(anagram.begin(), anagram.end());
	ifstream file("J:\\Documents\\download\\testList");
	list<string> wordList;

	if (file.is_open())
	{
		string word;
		while (getline(file, word))
		{
			//bool wordOk = true;
			//for each (char ch in anagram)
			//{
			//	if (word.find_first_of(ch) == std::string::npos)
			//	{
			//		wordOk = false;
			//		break;
			//	}
			//}
			if (word.find_first_not_of(anagram) == string::npos)
			{
				wordList.push_back(word);
			}
		}

		file.close();
	}
	time_t start = time(0);
	struct tm current;

	char* dt;
	ctime_s(dt, 26, &start);
	
	cout << dt;

	string phase = findPhase(wordList);

	time_t stop = time(0);
	ctime_s(dt, 26, &stop);
	cout << dt;

	cout << phase << "\n";
	//std::printf("%s \n", anagram);

	//for each (string word in wordList)
	//{
	//	cout << word << "\n";
	//	
	//}
	//cout << anagram;
	std::cin.get();
	return 0;
}




