#ifndef FATTABLE
#define FATTABLE

typedef struct{

    int* a;
public:
    void createTable(int tableSize){
        a = new int[tableSize];
    }
} FAT;

#endif // FATTABLE

