#include"stdio.h"
#include"filesystemdriver.h"
#include <iostream>

int main()
{
    FileSystemDriver driver("/home/maxfromperek/Downloads/v4.dat");
    char ch_key;
    char anyKey;
    bool bMENU = true;
    while(bMENU)
    {
        if(ch_key != '\n'){
            cout << "\033c";
            cout<<"***MENU***\n\n";
            cout<<"1 - print superblock info\n";
            cout<<"2 - print root directory \n";
            cout<<"3 - print FAT table \n";
            cout<<"4 - print file \n";
            cout<<"5 - print FS tree\n";
            cout<<"6 - pynkt menu 1\n";
            cout<<"0 - close driver\n";
        }
        switch((ch_key = getchar()))
        {
        case '1':
            cout << "\033c";
            driver.printSuperBlockInfo();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '2':
            cout << "\033c";
            driver.printRootDirectory();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '3':
            cout << "\033c";
            driver.printFAT();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '4':
            cout << "\033c";
            driver.printFile();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '5':
            cout << "\033c";
            driver.printFSTree();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '6':
            cout << "\033c";
            driver.saveFile();
            cout << "Press ENTER to continue...";
            anyKey = getchar();
            if(anyKey == '\n')
                anyKey = getchar();
            break;
        case '0':
            bMENU = false;
            cout<<"Exit from program\n";
            break;
        default:
            cout<<"Unsupported key was pressed\n";
            break;
        }
    }
    return 0;
}

