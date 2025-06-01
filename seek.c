#include "seek.h"
#include "headers.h"
void listFilesInDirectory(const char *dirPath, const char *search, int isFileFlag, int isDirectoryFlag,const char*basepath,int* countf,int* countd,char*changedir) 
{
DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
     struct dirent *entry;
     while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char filePath[PATH_MAX];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name); 
        // printf("%s",filePath);
        struct stat fileInfo;
        if (stat(filePath, &fileInfo) == -1) {
            perror("stat");
            continue;
        }
         char new[MAX_INPUT_LENGTH];
        char new1[MAX_INPUT_LENGTH];
        if(isFileFlag==0 && isDirectoryFlag==0){
             if ( S_ISREG(fileInfo.st_mode)) {
            if (strstr(entry->d_name, search) !=NULL) {
                (*countf)++;
                new[0]='.';
                strcpy(new+1,filePath+strlen(basepath));     
                printf("\033[0;32m%s\033[0m\n", new);
                strcpy(changedir,filePath);
            }
        }
          else  if ( S_ISDIR(fileInfo.st_mode)) {
            if (strcmp(entry->d_name, search) ==0) {
                (*countd)++;
                 new1[0]='.';
                  strcpy(new1+1,filePath+strlen(basepath));
                printf("\033[0;34m%s\033[0m\n", new1);
                strcpy(changedir,filePath);
            }
            listFilesInDirectory(filePath, search, isFileFlag, isDirectoryFlag,basepath,countf,countd,changedir);
        }
        }
          else if(isFileFlag==0 && isDirectoryFlag==1){
            if ( S_ISREG(fileInfo.st_mode)) {
            if (strstr(entry->d_name, search) !=NULL) {
                //printf("\033[0;32m%s\033[0m\n", filePath);
            }
        }

      else  if ( S_ISDIR(fileInfo.st_mode)) {
            if (strcmp(entry->d_name, search) ==0) {
                //printf("\033[0;34m%s\033[0m\n", filePath);
                (*countd)++;
                new1[0]='.';
                  strcpy(new1+1,filePath+strlen(basepath));
                printf("\033[0;34m%s\033[0m\n", new1);
                strcpy(changedir,filePath);
            }
            listFilesInDirectory(filePath, search, isFileFlag, isDirectoryFlag,basepath,countf,countd,changedir);
        }
         }
         else if(isFileFlag==1 && isDirectoryFlag==0){
             if ( S_ISREG(fileInfo.st_mode)) {
            if (strstr(entry->d_name, search) !=NULL) {
                //printf("\033[0;32m%s\033[0m\n", filePath);
                (*countf)++;
                 new[0]='.';
                strcpy(new+1,filePath+strlen(basepath));
                printf("\033[0;32m%s\033[0m\n", new);
               
                strcpy(changedir,filePath);
            }
        }

      else  if ( S_ISDIR(fileInfo.st_mode)) {
            if (strcmp(entry->d_name, search) ==0) {
                //printf("\033[0;34m%s\033[0m\n", filePath);
            }
            listFilesInDirectory(filePath, search, isFileFlag, isDirectoryFlag,basepath,countf,countd,changedir);
        }
         }
        
     }
        closedir(dir);

}
void seek(char* search,char* targetDir, int isFileFlag, int isDirectoryFlag,int Eflag)
{
    if(isFileFlag==1 && isDirectoryFlag==1)
       {
            printf("Invalid flags!\n");
            return;
        }
        int countf=0;
         int countd=0;
         char changedir[MAX_INPUT_LENGTH];
         printf("Searching for '%s' in '%s':\n", search, targetDir);
         listFilesInDirectory(targetDir, search, isFileFlag, isDirectoryFlag,targetDir,&countf,&countd,changedir);
         if(countf==0 && countd==0){
             printf("No match found!\n");}
            //  printf("%d %d\n",countd,countf);
    if(countf==0 && countd==1 && Eflag==1){ 
         if (access(changedir, X_OK) == -1) {
        perror("access");
        printf("Missing permissions for task!\n");
        
    }
    if (chdir(changedir) == -1) {
        perror("chdir");
    
    }
    
    }
     if(countf==1 && countd==0 && Eflag==1){
        char write2[MAX_INPUT_LENGTH];
        if(getcwd(write2,sizeof(write2))!=NULL){
            char* write;
        write=strrchr(changedir, '/');
        changedir[strlen(changedir)-strlen(write)]='\0';
        chdir(changedir);
        char write1[MAX_INPUT_LENGTH];
        strcpy(write1,write+1);
           FILE *file = fopen(write1, "r");
    if (file == NULL) {
        perror("fopen");
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s\n", buffer);
        }
    fclose(file);
        }
        chdir(write2);
    }
         return;
}