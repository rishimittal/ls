/* PRO.C (MAIN PROGRAM)*/
/* CONSISTS OF THE MAIN METHOD AND FILTER METHOD FOR RECURSIVE OPTION (-R) 
   DATA IS BEING PRNTED IN HERE ONLY.
*/
#include "proHeader.h"

using namespace std;


/* Scans the current directory and adds the all files on the List*/ 
vector<string> addFilesToList(vector<string> vp , const char* path){
	struct dirent **namelist;
	int n , i;
	
	//Usage of sys call	
	n = scandir(path ,&namelist , 0, alphasort);
	if( n < 0 ) {
		cout<<"my_ls : cannot open directory "<<path; // to be reviewed
		perror(0);
		cout<<endl;
	}
	else{
		//cout<<path<<":"<<endl;
		for(i = 0 ; i <= n - 1 ; i++) {
			//printf("FROM :::%s\n",namelist[i]->d_name);
			vp.push_back(namelist[i]->d_name);
			free(namelist[i]);
		}
		free(namelist);
	}
	//Call the function and sort the vector.
	sort(vp.begin(), vp.end() , my_basic_ls_sort);
return vp;
}

/**/
char* makePath(string fileName , const char* path) {
	string completePath;
	string pth(path);
		if(fileName.compare(pth) != 0) {
        		completePath.append(path);
			completePath.append("/");
			completePath.append(fileName); 
		}else {
		completePath.append(fileName);
		}
        char *toPass = new char[completePath.size() + 1];
        copy(completePath.begin(), completePath.end(), toPass);
        toPass[completePath.size()] = '\0';
	
return toPass;
}



/*SORTS EXTRA CONDITIONS : CALLED IN filter_S*/
bool my_func_to_sort_size(const FileDetails fd1 ,const FileDetails fd2){
	if(fd1.fileSize > fd2.fileSize)
		return true;
	else if(fd1.fileSize < fd2.fileSize)
		return false;
	else if(fd1.fileSize == fd2.fileSize)
		return my_basic_ls_sort(fd1.fileName ,fd2.fileName);//DOING BASIC SORT ,IF FILESIZES ARE SAME.
return false;	
}

/*SORTS EXTRA CONDITIONS : CALLED IN filter_S*/
bool my_func_to_sort_time(const FileDetails fd1 ,const FileDetails fd2){
        if(fd1.timeSort > fd2.timeSort)
                return true;
        else if(fd1.timeSort < fd2.timeSort)
                return false;
	else if(fd1.timeSort == fd2.timeSort)
		return my_basic_ls_sort(fd1.fileName ,fd2.fileName);//DOING BASIC SORT ,IF TIME IS SAME.
return false;
}

void printFileInfo(int lflag ,const char *path ,vector<FileDetails> sV){
	int k;
	struct stat sb;
	ssize_t rl; 
	char* resolvLink;
	int maxLinkLength = 1;
	if(lflag == 0) {
		for(k = 0 ;k < sV.size() ; k++) {

			//cout<<sV[k].fileName<<"\n";	
			colorText(sV[k].fileName , path);	
			cout<<endl;
		}		
	}else if(lflag != 0) {
		for(k = 0 ; k < sV.size() ; k++) {
			 char *writable = makePath(sV[k].fileName , path);
 			 if((lstat(writable, &sb)) == -1 ){
				perror("lstat");
			 }
			 if(fileType(sb) == 'l') { resolvLink = getResolveLink(sb, writable);}
			  
			 if(((int)log10(sV[k].noOfLinks)+1) > maxLinkLength){ maxLinkLength = (int)log10(sV[k].noOfLinks)+1 ;}
                         cout<<left<<setw(1)<<sV[k].fileType; //FileType
 			 cout<<left<<setw(9)<<sV[k].perms<<" "; //File Permissions
			 cout<<right<<setw(maxLinkLength)<<setfill(' ')<<sV[k].noOfLinks<<" "; //File Links
                         cout<<sV[k].userName<<" "; //userName
                         cout<<sV[k].groupName<<" "; //Group Name
 			 cout<<sV[k].fSize<<" "; //FileSize
  			 cout<<sV[k].moTime<<" "; //File last Mod Date
                         //cout<<sV[k].fileName<<" "; 
			 colorText(sV[k].fileName , path); //Used only to print file Names.
			 if(fileType(sb) == 'l'){ cout<<"->"<<resolvLink<<" "; }
			 cout<<endl;
                      //   cout<<ctime(&sV[k].timeSort)<<"\n";
                 }
		cout<<endl;
	}
	//sV.clear();
	//cout<<endl;
}

bool my_basic_ls_sort_recursive(const FileDetails left ,const FileDetails &right){	
	
for(string::const_iterator lit = left.fileName.begin(), rit = right.fileName.begin();lit != left.fileName.end() && rit != right.fileName.end(); ++lit,++rit) {	
		if(*lit == '.')lit++;
		if(*rit == '.')rit++;
		if(toupper(*lit) < toupper(*rit))
			return true;
		else if(toupper(*lit) > toupper(*rit))
			return false;
	}
return false;		
}

void filter_R(int lflag,int aflag ,int noflag ,int Sflag , int tflag ,const char* loc, vector<FileDetails> &superVector){
	int i, j, argcDir = 0;
	//vector<string> vp , op;
	vector<FileDetails> forLS;//new
	//struct stat fileA; //new
	FileDetails fd1 ;
	vector<FileDetails> vp, vs;
	int flag = 0;
	//vp =  addFilesToList(vp ,loc);  //vector<string> addFilesToList(vector<string> vp , const char* path)
	vp = superVector;
	//cout<<"chek1"<<"\n";
	for(j = 0 ; j < vp.size(); j++) {
		struct stat rFileAttrs; //= applyStat(rFileAttrs ,vp[j].fileName ,loc);
		char *writable = makePath(vp[j].fileName , loc);
                if((lstat(writable, &rFileAttrs)) == -1){ 
                        cout<<"my_ls: cannot open directory "<<loc;
                        perror(0);
			cout<<endl;
			continue;
	}		
  //               }else{

		//cout<<vp[j].fileName<<"\n";
		//cout<<fileType(rFileAttrs)<<"\n";
		if((fileType(rFileAttrs) == 'd' ) && (vp[j].fileName.compare(".") != 0 || vp[j].fileName.compare("..") != 0) ){
			//cout<<"chek2"<<"\n";
			argcDir++;
			string locName(loc);
			string fName = vp[j].fileName.append("/");
			string pName = "";
			pName.append(locName);
		//	cout<<"CHECK :"<<pName<<" "<<fName<<"\n";	
			//printFileInfo(vp);
			if(fName.compare("./") == 0 ||  fName.compare("../") == 0)
				continue;

			pName.append(fName);
			
			//PRINTS OUT PATH.
			cout<<pName<<": "<<endl;
				
			//vector<string> innerList;
			//innerList = addFilesToList(innerList , pName.c_str()); 
			//for(i = 0 ; i < innerList.size() ; i++){
			//	cout<<innerList[i]<<" ";
			//}
			//cout<<"chek3"<</"\n";	
			forLS = calling_R(fd1 ,lflag,aflag,noflag,Sflag,tflag,pName.c_str(),forLS);
			//printFileInfo(flag ,pName.c_str() ,forLS);
			//cout<<"\n";
			forLS.clear();
			//cout<<"chek4"<<"\n";
 
		//	vs = setFD(pName.c_str());
		//	for(i = 0 ; i < vs.size() ; i++ ){
		//		cout<<vs[i].fileName<<" ";
		//	}
			filter_R(lflag,aflag,noflag,Sflag,tflag,pName.c_str(),forLS);	
			
		}else { }
//	}
//		if(argcDir == 0) break;

	}	//cout<<"\n";
	//delete fd1;
}


vector<FileDetails> calling_R(FileDetails fd ,int lflag ,int aflag , int noflag , int Sflag ,int tflag , const char* path, vector<FileDetails> &superVector) {
	
	//vector<FileDetails> superVector;
 		
	if(aflag == 1) {
		//call filter_a , it sets the recursive_R
		superVector = filter_a(fd,path, superVector);
		//printFileInfo(lflag ,path, superVector);
		if(lflag == 1) filter_l(path,superVector ); /*Called when -l and a are set*/
		if(Sflag == 1) filter_S(path,superVector );/*Called when -S and a are set*/
		if(tflag == 1) filter_t(path,superVector );/*Called when -S and a are set*/
		printFileInfo(lflag , path , superVector);
		filter_R(lflag, aflag ,noflag ,Sflag ,tflag , path, superVector);
	} else  {
		superVector = filter_noOption(fd,path , superVector);
		if(lflag == 1) filter_l(path,superVector ); /*Called when -l and a are set*/
		if(Sflag == 1)  filter_S(path,superVector );/*Called when -S and a are set*/
		if(tflag == 1) filter_t(path,superVector );/*Called when -S and a are set*/
		printFileInfo(lflag , path , superVector);
		filter_R(lflag, aflag ,noflag ,Sflag ,tflag , path, superVector);	
	}

return superVector;
}

int main(int argc , char *argv[]) {

	//struct stat fileAttrs;
	int lflag = 0, aflag = 0, dflag = 0, Sflag = 0, tflag = 0, Rflag = 0 ;
        int noflag = 0 ,wflag = 0;
	int tempFlag = 0; /*Initially it is zero but if there are options in that case it is set.*/
	FileDetails fd1;
	int k;
	while(1) {
		char ch;
		
 		ch = getopt(argc , argv ,"ladStR");
		if(ch == -1) {
			//printf("finished searching\n");
			break;
		}	
		
		switch(ch) {
			case 'l': lflag = 1;tempFlag = 1;
				  break;
			case 'a': aflag = 1;tempFlag = 1;
				  break;
			case 'd': dflag = 1;tempFlag = 1;
                                  break;
			case 'S': Sflag = 1;tempFlag = 1;
                                  break;
			case 't': tflag = 1;tempFlag = 1;
                                  break;
			case 'R': Rflag = 1;tempFlag = 1;
                                  break;
			case '?': wflag = 1;tempFlag = 1;
				  break;
			}
		}
		if(tempFlag == 0) noflag = 1;
		//printf("l :%d, a :%d, d :%d, s :%d ,t :%d ,R :%d ,WR :%d ,NO :%d\n" , lflag, aflag, dflag, Sflag, tflag, Rflag,wflag, noflag);
		argc = argc -  optind;
		argv = argv +  optind;

		int i;
		const char* path = "./";
	if(argc > 0) {
	    for(i = 0 ;i < argc ; i++) {
		vector<FileDetails> superVector, hg;	
		path = argv[i];

		if(aflag == 1 && wflag == 0 && dflag == 0 && Rflag == 0) {			
			superVector = filter_a(fd1,path, superVector);
			if(lflag == 1) filter_l(path, superVector ); /*Called when -l and a are set*/
			if(Sflag == 1) filter_S(path, superVector );/*Called when -S and a are set*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/	
		}else if(noflag == 0 && wflag == 0 && dflag == 0 && Rflag == 0) {
			superVector = filter_noOption(fd1,path, superVector);
			if(lflag == 1) filter_l(path , superVector);/*Only -l is called*/
			if(Sflag == 1) filter_S(path, superVector );/*Only -S is called*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/
		}else if(dflag == 1 && wflag == 0){
			filter_d(path, superVector);
			if(lflag == 1) filter_l(path , superVector);/*Only -l is called*/
                        if(Sflag == 1) filter_S(path, superVector );/*Called when -S and a are set*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/
		}
		else if(noflag == 1 && wflag == 0 && Rflag == 0) {
		 	superVector = filter_noOption(fd1,path , superVector);
		
		}else if(Rflag == 1 && wflag == 0 && dflag == 0){
			//cout<<"in else1"<<"\n"; 
			hg = calling_R(fd1,lflag,aflag,noflag,Sflag,tflag,path, superVector);	
		}
		if(Rflag == 0 )printFileInfo(lflag ,path ,superVector);
		superVector.clear();
	    } //END FOR LOOP
	} //END IF LOOP
	else{
		vector <FileDetails> superVector,hg;
	
		if(aflag == 1 && wflag == 0 && dflag == 0 && Rflag == 0) {			
			superVector = filter_a(fd1,path, superVector);
			if(lflag == 1) filter_l(path, superVector ); /*Called when -l and a are set*/
			if(Sflag == 1) filter_S(path, superVector );/*Called when -S and a are set*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/	
		}else if(noflag == 0 && wflag == 0 && dflag == 0 && Rflag == 0) {
			superVector = filter_noOption(fd1,path, superVector);
			if(lflag == 1) filter_l(path , superVector);/*Only -l is called*/
			if(Sflag == 1) filter_S(path, superVector );/*Only -S is called*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/
		}else if(dflag == 1 && wflag == 0){
			superVector = filter_d(path, superVector);
			if(lflag == 1) filter_l(path , superVector);/*Only -l is called*/
                        if(Sflag == 1) filter_S(path, superVector );/*Called when -S and a are set*/
			if(tflag == 1) filter_t(path, superVector );/*Called when -S and a are set*/
		}
		else if(noflag == 1 && wflag == 0 && Rflag == 0) {
		 	superVector = filter_noOption(fd1,path , superVector);
		
		}else if(Rflag == 1 && wflag == 0 && dflag == 0){
		
			hg = calling_R(fd1,lflag,aflag,noflag,Sflag,tflag,path, superVector); 
		}
		
	if(Rflag == 0) printFileInfo(lflag ,path ,superVector);	
	superVector.clear();
	}
return 0;
}
