// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>

#define BUFFER_SIZE 256

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" {
    __declspec(dllexport) int scanFile(const char* filePath, const char* firstChar, const char* secondChar) {
        std::ifstream fileDescriptor;//create a file descriptor for future use
        fileDescriptor.open(filePath, std::ios::in);//opening the file locaited in the passed argument "filePath"

        //variables to keep trace of the result
        char currChar;
        int firstCharCounter = 0;
        int secondCharCounter = 0;

        if (!fileDescriptor.is_open()) {//the file was not opend 
            std::cerr << "Unable to open file in location " << filePath << std::endl;
            return EXIT_FAILURE;
        }
        else {
            while (fileDescriptor.get(currChar)) {//extract every char and check if it equals the given chars
                if (currChar == *firstChar) {//match the first char
                    firstCharCounter++;
                }
                else if (currChar == *secondChar) {//match the second char
                    secondCharCounter++;
                }
            }
        }

        if ((firstCharCounter - secondCharCounter) >= 0 && firstCharCounter > 0) {
            return 1;//return one if the first and second char have equal caunters, and are diffrent from zero
        }

        return 0;
    }
}

extern "C" {
    __declspec(dllexport) int scanFile2(const char* filePath, const char* firstChar, const char* secondChar){
        HANDLE hFile = CreateFileA(
            filePath,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        //variables to keep trace of the result
        int firstCharCounter = 0;
        int secondCharCounter = 0;
        int currChar = 0;

        if (hFile == INVALID_HANDLE_VALUE) {//file was not reachable 
            std::cout << "file was not opend, sorry not working :-(" << std::endl;
        }

        
        char buffer[BUFFER_SIZE] = {'\0'};//setting the buffer for the data collection, init with all null chars
        DWORD bytsToRead = BUFFER_SIZE-1;//the amount of data to collect
        DWORD bytsWereRead = 0;//how much was read 
        BOOL result = ReadFile(hFile, (void*)buffer, bytsToRead, &bytsWereRead, NULL);// retrieve the data from the file 

        if (bytsWereRead > 0) {//data was retrieved 
            for (;buffer[currChar] != '\0'; currChar++) {//traverse through all the data
                if (buffer[currChar] == *firstChar) {//match the first char
                    firstCharCounter++;
                }
                else if (buffer[currChar] == *secondChar) {//match the second char
                    secondCharCounter++;
                }
            }
        }

        CloseHandle(hFile);//close the file handler 
        if ((firstCharCounter - secondCharCounter) >= 0 && firstCharCounter > 0) {
            return 1;//return one if the first and second char have equal caunters, and are diffrent from zero
        }
        
        return 0;
    }
}


