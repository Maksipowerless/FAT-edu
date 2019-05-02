#include "filesystemdriver.h"

FileSystemDriver::FileSystemDriver(const char* fsPath){

    _superBlock = new SuperBlock();
    readFileSystem(fsPath);
    _fsPath = fsPath;
    _dataPointer = sizeof(SuperBlock) + _superBlock->_tableSize + _superBlock->_rootDirectorySize*sizeof(File);
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

    File* file = new File;
    for(int i = 0; i < _superBlock->_rootDirectorySize; i++){
        fread(file, sizeof(File), 1, fs);
        _files.insert(_files.end(), *file);
    }


    delete iPointer;
    delete file;
    fclose(fs);
}

void FileSystemDriver::printSuperBlockInfo(){

    cout << "***SUPERBLOCK INFO***:" << "\n\n";
    cout << "Block size:" << _superBlock->_blockSize << "\n";
    cout << "Encryption shift:" << _superBlock->_encryptionShift << "\n";
    cout << "Root directory size:" << _superBlock->_rootDirectorySize << "\n";
    cout << "FAT table size:" << _superBlock->_tableSize << "\n";
}


void FileSystemDriver::printFAT(){

    cout << "***FAT table***:" << "\n\n";
    for(int i = 0; i < _fat.size(); i++){
        cout << i << ": " << _fat.at(i) << "\n";
    }
}

void FileSystemDriver::printRootDirectory(){

    cout << "***Root directory***:" << "\n\n";
    for(int i = 0; i < _files.size(); i++){
        char type;
        if (_files.at(i)._type == 1)
            type = 'd';
        else
            type = 'f';

        cout << type << ": " <<_files[i]._name << "  first block is " << _files[i]._firstBlock << "\n";
    }
}

void FileSystemDriver::printFSTree(){
    cout << "***FILE SYSTEM TREE***:" << "\n\n";
    for(int i = 0; i < _files.size(); i++){
        if(strcmp(_files[i]._name, "")){
            if(_files[i]._type == 0)
                cout << "f: " << _files[i]._name << "\n";
            else{
                cout << "d: " << _files[i]._name << "\n";
                recurciveDirectoryPrint(1,  _files[i]._firstBlock);
            }
        }
    }
}

void FileSystemDriver::recurciveDirectoryPrint(int shift, int firstBlock){

    vector<int> fileChainFAT = getBlockChainFAT(firstBlock);
    string spaces;
    for(int i = 0; i < shift; i++)
        spaces.append("  ");

    for(int i = 0; i < fileChainFAT.size(); i++){
        FILE* fs;
        if((fs=fopen(_fsPath, "rb+"))==NULL){
            printf("Cannot open file system.\n");
            exit(1);
        }

        fseek(fs, _dataPointer + _superBlock->_blockSize*fileChainFAT[i], SEEK_SET); // сместить указатель на нужный блок
        File* file = new File;

        for(int i = 0; i < _superBlock->_blockSize/sizeof(File); i++){
            fread(file, sizeof(File), 1, fs);
            if(strcmp(file->_name, "")){
                if(file->_type == 0)
                    cout << spaces << "f: " << file->_name << "\n";
                else{
                    cout << spaces << "d: " << file->_name << "\n";
                    recurciveDirectoryPrint(shift +  1,  file->_firstBlock);
                }
            }
        }
        delete file;
        fclose(fs);
    }
}

vector<int> FileSystemDriver::getBlockChainFAT(int firstBlock){

    vector<int> chain;
    chain.insert(chain.end(), firstBlock);
    int value = _fat.at(firstBlock);
    while(value != -1){
        value = _fat.at(value);
        chain.insert(chain.end(), value);
    }
    return chain;
}

void FileSystemDriver::printFile(){

    for(int i = 0; i < _files.size(); i++){
        string tmp = _files[i]._name;
        if(tmp.find(".txt") != -1){
            vector<int> fileChain = getBlockChainFAT(_files[i]._firstBlock);
            string text;
            for(int i = 0; i < fileChain.size(); i++){
                FILE* fs;
                if((fs=fopen(_fsPath, "rb+"))==NULL){
                    printf("Cannot open file system.\n");
                    exit(1);
                }

                fseek(fs, _dataPointer + _superBlock->_blockSize*fileChain[i], SEEK_SET); // сместить указатель на нужный блок
                char* cPointer = new char;

                for(int i = 0; i < _superBlock->_blockSize; i++){
                    fread(cPointer, sizeof(char), 1, fs);
                    if(*cPointer == 0)
                        break;
                    char symb = *cPointer - _superBlock->_encryptionShift;
                    text += symb;
                }

                delete cPointer;
                fclose(fs);
            }
            cout << text;
        }

    }

}


void FileSystemDriver::saveFile(){
    vector<File> regFiles;

    for(int i = 0; i < _files.size(); i++){ // поиск всех фвйлов
        if(strcmp(_files[i]._name, "")){
            if(_files[i]._type == 0)
                regFiles.insert(regFiles.end(), _files[i]);
            else{
                recurciveFindFiles(_files[i]._firstBlock, regFiles);
            }
        }
    }

    char command[20]; // запрашивает у пользователя имя файла, есди файл есть сохраняет его
    bool flag = true;
    while(flag){
        cout << "***SAVE FILE***:" << "\n\n";
        for(int i = 0; i < regFiles.size(); i++){
            cout << i+1 << ": " << regFiles[i]._name << "\n";
        }
        cout << "Enter the file name. For exit enter 'quit'\n";
        cin >> command;
        if(!strcmp(command, "quit"))
            flag = false;
        else{
            string fileName;
            fileName += command;
            for(int i = 0; i < regFiles.size(); i++){
                if(!strcmp(regFiles[i]._name, command )){
                    vector<int> fileChain = getBlockChainFAT(regFiles[i]._firstBlock);

                    string path = "/home/maxfromperek/";
                    path += regFiles[i]._name;
                    ofstream myfile;
                    myfile.open(path.c_str());

                    for(int j = 0; j < fileChain.size(); j++){
                        FILE* fs;
                        if((fs=fopen(_fsPath, "rb+"))==NULL){
                            printf("Cannot open file system.\n");
                            exit(1);
                        }

                        fseek(fs, _dataPointer + _superBlock->_blockSize*fileChain[j], SEEK_SET); // сместить указатель на нужный блок
                        char* cPointer = new char;

                        for(int i = 0; i < _superBlock->_blockSize; i++){
                            fread(cPointer, sizeof(char), 1, fs);

                            if(fileName.find(".jpg") == -1 && *cPointer == 0)
                                    break;
                            char symb = *cPointer - _superBlock->_encryptionShift;
                            myfile << symb;

                        }

                        delete cPointer;
                        fclose(fs);

                    }
                    myfile.close();
                    break;
                }
            }
        }

        cout << "\033c";
    }
}

void FileSystemDriver::recurciveFindFiles(int firstBlock, vector<File>& regFiles){

    vector<int> fileChainFAT = getBlockChainFAT(firstBlock);
    for(int i = 0; i < fileChainFAT.size(); i++){
        FILE* fs;
        if((fs=fopen(_fsPath, "rb+"))==NULL){
            printf("Cannot open file system.\n");
            exit(1);
        }

        fseek(fs, _dataPointer + _superBlock->_blockSize*fileChainFAT[i], SEEK_SET); // сместить указатель на нужный блок
        File* file = new File;

        for(int i = 0; i < _superBlock->_blockSize/sizeof(File); i++){
            fread(file, sizeof(File), 1, fs);
            if(strcmp(file->_name, "")){
                if(file->_type == 0)
                {
                    regFiles.insert(regFiles.end(), *file);
                }
                else{
                    recurciveFindFiles(file->_firstBlock, regFiles);
                }
            }
        }
        delete file;
        fclose(fs);
    }
}


































