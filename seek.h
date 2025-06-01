
#ifndef SEEK_H
#define SEEK_H

#include "headers.h"  // Includes all necessary standard headers

void listFilesInDirectory(const char *dirPath, const char *search, int isFileFlag, int isDirectoryFlag,const char*basepath,int* countf,int* countd,char*changedir) ;
void seek(char* search,char* targetDir, int isFileFlag, int isDirectoryFlag,int Eflag);
#endif // REVEAL_H