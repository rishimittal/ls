/* FILTER FILE */
/* CONSISTS OF METHODS USED FOR THE FILTERING THE RESULTS BASED ON THE OPTIONS 
   USED IN LS */
#include "proHeader.h"



vector<FileDetails> filter_a(FileDetails fd ,const char* path, vector<FileDetails> &superVector){

         //FileDetails fd;
         int j;
         struct stat fileAttrs;
         string fileName;
                /**/
                                vector<string> vp; /*Vector defined to store fileNames.*/
                                vp = addFilesToList(vp ,path);
                                //printList(fileAttrs,vp ,path);
                                if(!vp.empty()) {
                                        //cout<<path<<":"<<endl;
                                        for(j = 0 ; j < vp.size() ; j++) { //Loop for file in path.
                                                //fileAttrs = applyStat(fileAttrs , vp[j] ,path);
                                                char *writable = makePath(vp[j] , path);
                                                if((lstat(writable, &fileAttrs)) == -1) {
                                                        cout<<"my_ls: cannot access"<<path;
                                                        perror(0);
                                                        cout<<endl;
                                                }else{
                                                fileName = getColouredPrint(fileAttrs , vp[j]);
                                                //cout<<fileName<<" ";

                                                fd.fileName = fileName;
                                                superVector.push_back(fd);
                                                }
                                                //cout<<superVector[j].fileName<<" ";
                                        }
                                       //cout<<endl;
                        }
return superVector;
}

vector<FileDetails> filter_noOption(FileDetails fd,const char* path, vector<FileDetails> &superVector){

         //FileDetails fd;
         struct stat fileAttrs;
         int j ;
         string fileName;
         //int largest_len_of_string = 0;
                                /**/
                                vector<string> vp; /*Vector defined to store fileNames.*/
                                vp = addFilesToList(vp ,path);
                                //printList(fileAttrs,vp ,path);
                                if(!vp.empty()) {
                                        //cout<<path<<":"<<endl;
                                        for(j = 0 ; j < vp.size() ; j++) { //Loop for file in path.
                                                //fileAttrs = applyStat(fileAttrs , vp[j] ,path);
                                                char *writable = makePath(vp[j] , path);
                                                 if((lstat(writable, &fileAttrs)) == -1) {
                                                         cout<<"my_ls: cannot access"<<path;
                                                         perror(0);
                                                         cout<<endl;
                                                  }else{
                                                        fileName = getColouredPrint(fileAttrs , vp[j]);
                                                        char dot = fileName.at(0);
                                                        //cout<<fileName<<" ";
                                                        //fd.path = path
                                                        if(dot != '.') {
                                                                fd.fileName = fileName;
                                                                superVector.push_back(fd);
                                                       }
                                                }
                                        }
                                        //cout<<endl;
                                }
        //cout<<largest_len_of_string<<endl;
return superVector;
}


/*LIST IN LONG LISTING FORMAT*/
void filter_l(const char* path, vector<FileDetails> &superVector){
        struct stat fileAttrs;
        long long total = 0;
        int j;
                                if(!superVector.empty()){
                                        for(j = 0 ; j < superVector.size() ; j++) { //Loop for file in path.    
                                                //fileAttrs = applyStat(fileAttrs , superVector[j].fileName ,path);     
                                                char *writable = makePath(superVector[j].fileName , path);
                                                 if((lstat(writable, &fileAttrs)) == -1) {
                                                         cout<<"my_ls: cannot access"<<path;
                                                         perror(0);
                                                         cout<<endl;
                                                 }else{
                                                        superVector[j].fileType = fileType(fileAttrs); // File Type
                                                        superVector[j].perms = findAndPrintPermissions(fileAttrs); //Permisssions
                                                        superVector[j].noOfLinks = findLinks(fileAttrs);//hardLinks
                                                        superVector[j].userName = getUserName(fileAttrs); // UserName
                                                        superVector[j].groupName = getGroupName(fileAttrs); // groupName
                                                        superVector[j].fSize = getFileSize(fileAttrs); // fileSize
                                                        superVector[j].moTime = getTime(fileAttrs); //time
                                                        total += getTotalBlocks(fileAttrs);
                                                }
                                        }
                                cout<<"total "<<(total/2)<<endl;
                        }else {cout<<"total"<<total<<endl;}

}


/* SORT BY FILE SIZE */
void filter_S( const char* path, vector<FileDetails> &superVector){
        struct stat fileAttrs;
        int j;
                                if(!superVector.empty()){
                                        for(j = 0 ; j < superVector.size() ; j++) { //Loop for file in path.
                                                //fileAttrs = applyStat(fileAttrs , superVector[j].fileName ,path);
                                                char *writable = makePath(superVector[j].fileName , path);
                                                 if((lstat(writable, &fileAttrs)) == -1) {
                                                         cout<<"my_ls: cannot access"<<path;
                                                         perror(0);
                                                         cout<<endl;
                                                  }else{
                                                        superVector[j].fileSize = getFsSort(fileAttrs);
                                                }
                                        }
                                        //cout<<endl;
                                }
        //VECTOR SORTING
        sort(superVector.begin() ,superVector.end() , my_func_to_sort_size);
}


vector<FileDetails> filter_d( const char* path, vector<FileDetails> &superVector){

               FileDetails fd;
               struct stat fileAttrs ;// applyStat(fileAttrs , "" , path);
               char *writable = makePath("" , path);
               if((lstat(writable, &fileAttrs)) == -1) {
                         cout<<"my_ls: cannot access"<<path;
                         perror(0);
                         cout<<endl;
                }else{
                        fd.fileName = path;
                        superVector.push_back(fd);
                }
return superVector;
}


void filter_t(const char* path, vector<FileDetails> &superVector) {
        int j;
        struct stat fileAttrs;

         if(!superVector.empty()){
                 for(j = 0 ; j < superVector.size() ; j++) { //Loop for file in path.
                        //fileAttrs = applyStat(fileAttrs , superVector[j].fileName ,path);
                        char *writable = makePath(superVector[j].fileName, path);
                        if((lstat(writable, &fileAttrs)) == -1) {
                                 cout<<"my_ls: cannot access"<<path;
                                 perror(0);
                                 cout<<endl;
                         }else{
                                superVector[j].timeSort = getTimeSort(fileAttrs);
                        }
                }
                  //cout<<endl;
          }
        //VECTOR SORTING
        sort(superVector.begin() ,superVector.end() , my_func_to_sort_time);
}

