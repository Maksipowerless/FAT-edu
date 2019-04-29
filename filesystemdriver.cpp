#include "filesystemdriver.h"

FileSystemDriver::FileSystemDriver(const char* fsPath){

    _superBlock = new SuperBlock();
    readFileSystem(fsPath);
}

FileSystemDriver::~FileSystemDriver(){
    delete _superBlock;
}

void FileSystemDriver::readFileSystem(const char* fsPath){
    FILE* fs;
    if((fs=fopen(fsPath, "rb+"))==NULL){
        printf("Cannot open file system.\n");
        exit(1);
    }

    fread(_superBlock, sizeof(SuperBlock), 1, fs); // считать суперблок
    int blocksCount = _superBlock->_tableSize/sizeof(int); // кол-во блоков в таблице fat
    int* iPointer = new int;
    for (int i = 0; i < blocksCount; i++){
        fread(iPointer, 4, 1, fs);
        if(i%2 != 0)
            _fat.insert(_fat.end(), *iPointer);
    }

    char* cPointer = new char;
    string str;
    File file;
    for(int i = 0; i < _superBlock->_rootDirectorySize; i++){
        for(int j = 0; j < _fileNameLength; j++){
            fread(cPointer, 1, 1, fs);
            str += *cPointer;
        }

        file._name = str;
        str.clear();
        fread(iPointer, 4, 1, fs);
        file._firstBlock = *iPointer;
        fread(iPointer, 4, 1, fs);
        file._type = *iPointer;
        _files.insert(_files.end(), file);
    }

//    for(int j = 0; j < 1000000; j++){
//        fread(cPointer, 1, 1, fs);
//        char a = *cPointer;
//        if (a!=0){
//            a = a -5;
//            cout << a-5;
//        }
//    }


    delete iPointer;
    delete cPointer;
    fclose(fs);
}

void FileSystemDriver::printSuperBlockInfo(){

    cout << "***SUPERBLOCK INFO***:" << "\n";
    cout << "Block size:" << _superBlock->_blockSize << "\n";
    cout << "Encryption shift:" << _superBlock->_encryptionShift << "\n";
    cout << "Root directory size:" << _superBlock->_rootDirectorySize << "\n";
    cout << "FAT table size:" << _superBlock->_tableSize << "\n";
}


void FileSystemDriver::printFAT(){

    cout << "***FAT table***:" << "\n";
    for(int i = 0; i < _fat.size(); i++){
        cout << i+1 << ": " << _fat.at(i) << "\n";
    }
}

void FileSystemDriver::printRootDirectory(){

    cout << "***Root directory***:" << "\n";
    for(int i = 0; i < _files.size(); i++){
        char type;
        if (_files.at(i)._type == 1)
            type = 'd';
        else
            type = 'f';

        cout << type << ": " <<_files[i]._name << "  first block is " << _files[i]._firstBlock << "\n";
    }
}

void FileSystemDriver::printFileBlockChain(){

    int value = 289;
    int savedValue = value;
    while(value != -1){
        savedValue = value;
        value = _fat.at(value - 1);
        while(value == 0){
            savedValue++;
            value = _fat.at(savedValue - 1);
        }


        cout << value << '\n';

    }
}










































