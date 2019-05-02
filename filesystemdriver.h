#ifndef FILESYSTEMDRIVER
#define FILESYSTEMDRIVER
#include "superblock.h"
#include "file.h"
#include "fattable.h"
#include <stdio.h>
#include"stdlib.h"
#include<iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <fstream>


using namespace std;

class FileSystemDriver{
    
    SuperBlock* _superBlock;
    vector<File> _files;
    vector<int> _fat;
    const char* _fsPath;
    int _dataPointer;

    void readFileSystem(const char* fsPath); // считывает файловую систему

public:
     FileSystemDriver(const char* fsPath);
     ~FileSystemDriver();
     void printSuperBlockInfo();
     void printFAT();
     void printRootDirectory();
     void printFSTree();
     void printFile();
     vector<int> getBlockChainFAT(int firstBlock);
     void recurciveDirectoryPrint(int shift, int firstBlock);
     void saveFile();
     void recurciveFindFiles(int firstBlock, vector<File>& regFiles);

};

#endif // FILESYSTEMDRIVER

