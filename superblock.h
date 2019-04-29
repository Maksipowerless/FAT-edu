#ifndef SUPERBLOCK
#define SUPERBLOCK

typedef struct{

    unsigned _blockSize;
    unsigned _tableSize;
    unsigned _rootDirectorySize;
    unsigned _encryptionShift;
} SuperBlock;

#endif // SUPERBLOCK

