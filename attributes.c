/* ATTRIBUTES.C */
/* CONSISTS OF THE BASC FUNCTIONS USED FOR THE RETRIEVAL OF THE INFORMATION ABOUT THE FILE AND
   SORTING THOSE FILES FOR THE OPTIONS LIKE -S & -t .
*/

#include "proHeader.h"


/*This function applies the explicit sorting criteria to the sort function of the vectors*/
bool my_basic_ls_sort(const string &left ,const string &right){ 
        
        for(string::const_iterator lit = left.begin(), rit = right.begin();lit != left.end() && rit != right.end(); ++lit,++rit) {      
                if(*lit == '.')lit++;
                if(*rit == '.')rit++;
                if(toupper(*lit) < toupper(*rit))
                        return true;
                else if(toupper(*lit) > toupper(*rit))
                        return false;
        }
return false;
}

/* This Method return the type of the file being found.*/
char fileType(struct stat fileAttrs){
        char c ;

        // TO FIND THE FILE TYPE
        switch(fileAttrs.st_mode & S_IFMT) {
                case S_IFREG: c = '-'; // Regular file
                              break;
                case S_IFDIR: c = 'd'; // Directory
                              break;
                case S_IFCHR: c = 'c'; // Character device File
                              break;
                case S_IFBLK: c = 'b'; // Block Device File
                              break;
                case S_IFSOCK:c = 's'; // Local Domain Socket
                              break;
                case S_IFIFO: c = 'p'; // Named Pipe
                              break;
                case S_IFLNK: c ='l';  //Symbolic Link
                              break;
                default:      c = 'o'; // Not Any of Above
                              break;
        }
        return c;
}

/*To find the permission of the file*/
string findAndPrintPermissions(struct stat fileAttrs) {
        char arr[10];
        arr[0] = (fileAttrs.st_mode & S_IRUSR) ? 'r' :'-';
        arr[1] = (fileAttrs.st_mode & S_IWUSR) ? 'w' : '-';
        arr[2] = (fileAttrs.st_mode & S_IXUSR) ? 'x' : '-';
        arr[3] = (fileAttrs.st_mode & S_IRGRP) ? 'r' : '-';
        arr[4] = (fileAttrs.st_mode & S_IWGRP) ? 'w' : '-';
        arr[5] = (fileAttrs.st_mode & S_IXGRP) ? 'x' : '-';
        arr[6] = (fileAttrs.st_mode & S_IROTH) ? 'r' : '-';
        arr[7] = (fileAttrs.st_mode & S_IWOTH) ? 'w' : '-';
        arr[8] = (fileAttrs.st_mode & S_IXOTH) ? 'x' : '-';
        arr[9] = '\0';
        string str(arr);
return str;
}

/*To find the Number of hard links on a file*/
long int findLinks(struct stat fileAttrs) {
        long int n;
         //TO FIND NUMBER OF LINKS
        n = fileAttrs.st_nlink;
        return n;
}

/*Returns the userName*/
string getUserName(struct stat fileAttrs){
        struct passwd *pwd = getpwuid((long)fileAttrs.st_uid);
        return pwd->pw_name;
}

/*Returns the groupName*/
string  getGroupName(struct stat fileAttrs){
        struct group *grp = getgrgid((long)fileAttrs.st_gid);
       return grp->gr_name;
}

/*Returns the fileSze to display*/
string getFileSize(struct stat fileAttrs){
        string rValue;

        stringstream ss, sd;
        if(fileType(fileAttrs) == 'c' || fileType(fileAttrs) == 'b' ) {
                ss << (long)major(fileAttrs.st_rdev);
                rValue.append(ss.str());
                rValue.append(",  ");
                sd << (long)minor(fileAttrs.st_rdev);
                rValue.append(sd.str());
        }else {
                ss << (long)fileAttrs.st_size;
                rValue.append(ss.str());
        }
return rValue;
}

//Returns the number of blocks
long long int getTotalBlocks(struct stat fileAttrs){
     return (long long)fileAttrs.st_blocks;
}

/*Returns the fileSize to Sort*/
long long int getFsSort(struct stat fileAttrs){
        return (long long)fileAttrs.st_size;
}
/*Returns the time to sort*/
time_t getTimeSort(struct stat fileAttrs) {
        return fileAttrs.st_mtime;
}
/*Returns the time in MON DAY HH:MM format*/
string getTime(struct stat fileAttrs){
        char buf[30];
        /*Recieves time from stat ,pass through localtime*/
        struct tm* modtime = localtime(&(fileAttrs.st_mtime));
        /*formats the string*/
        strftime(buf ,30 ," %b %d %R",modtime);
        /*Converts char array to string to pass it on.*/
        string tTime(buf);
return tTime;
}


string getColouredPrint(struct stat fileAttrs , string fname){
        char color;
        string fileName;

        if (fileType(fileAttrs) == 'd' ){
                fileName = fname;
        }
        else if(fileType(fileAttrs) == 'c') {
                 fileName = fname;
        }
        else if(fileType(fileAttrs) == 'b'){
                 fileName = fname;
        }
        else if(fileType(fileAttrs) == 'p'){
                 fileName = fname;
        }
        else if(fileType(fileAttrs) == 's'){
                 fileName = fname;
        }
        else if(fileType(fileAttrs) == 'l'){
                 fileName = fname;
        }
        else if(fileType(fileAttrs) == '-'){
                 fileName = fname;
        }
return fileName;
}


char* getResolveLink(struct stat fileAttrs,char* fileNamePath){
	char * resolName;
	ssize_t rl;
	
	resolName = (char*)malloc(fileAttrs.st_size + 1);	

	if(resolName == NULL) printf("Insufficient Menory.");
	
	rl = readlink(fileNamePath , resolName , fileAttrs.st_size);
	
	if(rl  < 0) perror("lstat");
	if(rl > fileAttrs.st_size ) printf("symlink sze increases d .");
	
	resolName[fileAttrs.st_size] = '\0';

return resolName;
}


void colorText(string text, const char* path) {
	
	struct stat sb;
	char *writable = makePath(text , path);
        if((lstat(writable, &sb)) == -1 ){
                perror("lstat");
        }
	
	if (fileType(sb) == 'd' ){
                cout<<LGTBLUE<<BOLD<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == 'c') {
                cout<<LGTYELLOW<<BOLD<<DARK_GREY<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == 'b'){
                cout<<LGTYELLOW<<BOLD<<DARK_GREY<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == 'p'){
                cout<<YELLOW<<DARK_GREY<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == 's'){
                cout<<MAGENTA<<BOLD<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == 'l'){
                cout<<LGTCYAN<<BOLD<<text<<RESET_COLOR<<" ";
        }
        else if(fileType(sb) == '-'){
                cout<<text<<" ";
        }
}

 
