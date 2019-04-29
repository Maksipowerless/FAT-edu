#ifndef FILESYSTEMDRIVER
#define FILESYSTEMDRIVER
#include "superblock.h"
#include "file.h"
#include "fattable.h"
#include"stdio.h"
#include"stdlib.h"
#include<iostream>
#include <vector>

using namespace std;

class FileSystemDriver{
    
    SuperBlock* _superBlock;
    vector<File> _files;
    vector<int> _fat;
    const int _fileNameLength = 12;

    void readFileSystem(const char* fsPath); // считывает файловую систему

public:
     FileSystemDriver(const char* fsPath);
     ~FileSystemDriver();
     void printSuperBlockInfo();
     void printFAT();
     void printRootDirectory();
     void printFileBlockChain();

};

#endif // FILESYSTEMDRIVER

