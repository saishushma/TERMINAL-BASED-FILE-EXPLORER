#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <termios.h>
#include<fcntl.h>
#include<stdlib.h>
#define clearline() printf("\033[K")
using namespace std;
//========================variables=============
stack<string> stackback;
stack<string> stackfront; 
vector<string> files;
vector<vector<string>> files_info;
char path[20580];
char rootp[20560];
int win_size;
int cursor_pos;
vector<string> com;
struct termios old = {0};
//==============functions===================
void to_normal_mode(string currpath);
void  get_files(string currpath);
void get_filesinfo(string currpath);
void to_rawmode();
void clrscr();
void set_position(int x,int y);
void print_details(int vecstart,int vecend);
void set_winsize();
//void printvec(int i);
void enable_scroll(int top,int bottom,int vecstart,int vecend);
//void level_up();
void to_command_mode();
string getinput();
void parse_command(string command);
int change_path();
void perform();
void fun_goto();
int fun_search(string dirname,string filename);
void fun_rename();
void fun_createfile();
void fun_createdir();
void copyfile(string filename,string dest);
void fun_copy();
void copydir(string dirname,string dest);
int deletefile(string file);
void deletedir(string dest);
void fun_move();
void quitp();
//===================================
void to_normal_mode(string currpath)
{
strcpy(path,currpath.c_str());
get_files(currpath);
get_filesinfo(currpath);
//to_rawmode();
set_winsize();
//win_size=4;
int vecstart=0,vecend=win_size-1;
print_details(vecstart,vecend);
enable_scroll(0,win_size-1,0,files.size()-1);
}
//=========================
void  get_files(string currpath)
{
files.clear();
DIR *dir; 
struct dirent *diread;
char f[2560];
strcpy(f, currpath.c_str());
if ((dir = opendir(f)) != nullptr) {
	 while ((diread = readdir(dir)) != nullptr) {
		files.push_back(diread->d_name);
 		}
        closedir (dir);
    } else {
        perror ("opendir");
        //return EXIT_FAILURE;
    }
    sort(files.begin(),files.end());

}
//==================================================
void get_filesinfo(string currpath)
{
files_info.clear();
DIR *dir; 
struct dirent *diread;
char f[2560];
strcpy(f, currpath.c_str());
if ((dir = opendir(f)) != nullptr) {
//         while ((diread = readdir(dir)) != nullptr) {
               // files.push_back(diread->d_name);
int count=0;
 for (auto file : files){ 
struct stat s1;
char f1[2560];
string s2=path;
string str;
str=s2+"/"+file;
strcpy(f1, str.c_str());
string per="";

if(stat(f1,&s1)==0)
 {
 if(S_ISDIR(s1.st_mode))
 per=per+"d";
 else per=per+"-";
 if((s1.st_mode &  S_IRUSR)&&(s1.st_mode & S_IREAD))
  per=per+"r";
  else per=per+"-";
if((s1.st_mode &  S_IWUSR)&&(s1.st_mode & S_IWRITE))
 per=per+"w";
else per=per+"-";
if((s1.st_mode &  S_IXUSR)&&(s1.st_mode & S_IEXEC))
per=per+"x";
else per=per+"-";
if((s1.st_mode &  S_IRGRP)&&(s1.st_mode & S_IREAD))
per=per+"r";
else per=per+"-";
if((s1.st_mode &  S_IWGRP)&&(s1.st_mode & S_IWRITE))
per=per+"w";
else per=per+"-";
if((s1.st_mode &  S_IXGRP)&&(s1.st_mode & S_IEXEC))
 per=per+"x";
 else per=per+"-";
if((s1.st_mode &  S_IROTH)&&(s1.st_mode & S_IREAD))
 per=per+"r";
 else per=per+"-";
if((s1.st_mode &  S_IWOTH)&&(s1.st_mode & S_IWRITE))
 per=per+"w";
 else per=per+"-";
if((s1.st_mode &  S_IXOTH)&&(s1.st_mode & S_IEXEC))
 per=per+"x";
 else per=per+"-";
//////
struct passwd *pw = getpwuid(s1.st_uid);
struct group  *gr = getgrgid(s1.st_gid);
/////////
auto dt=*(gmtime(&s1.st_mtime));
string mdate="";
mdate=mdate+to_string(dt.tm_mday);
mdate=mdate+"-";
mdate=mdate+to_string(dt.tm_mon);
mdate=mdate+"-";
mdate=mdate+to_string(dt.tm_year+1900);
mdate=mdate+" ";
mdate=mdate+to_string(dt.tm_hour);
mdate=mdate+":";
mdate=mdate+to_string(dt.tm_min);
mdate=mdate+":";
mdate=mdate+to_string(dt.tm_sec);
mdate=mdate+"";
string finfo="";
if(file.size()>20)
{
file=file.substr(0,20);
file=file+"...";
}
else if(file.size()<=20)
{
for(int i=file.size();i<=20;i++)
file=file+" ";
}
//printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",file,per,to_string(s1.st_size),(pw->pw_name),(gr->gr_name),mdate);
//finfo=finfo+file+"\t\t"+per+"\t"+to_string(s1.st_size)+"\t"+(pw->pw_name)+"\t"+(gr->gr_name)+"\t"+mdate;
//cout<<finfo<<endl;
//files_info.push_back(finfo);
//cout<<"hello";
vector<string> v1;
v1.clear();
v1.push_back(file);
v1.push_back(per);
v1.push_back(to_string(s1.st_size));
v1.push_back(pw->pw_name);
v1.push_back(gr->gr_name);
v1.push_back(mdate);
files_info.push_back(v1);
count++;
}
}
        closedir (dir);
    } 
    else {
        perror ("opendir");
        //return EXIT_FAILURE;
    }

}
//=======================================
void to_rawmode()
{

if (tcgetattr(0, &old) < 0)
	perror("tcsetattr()");

old.c_lflag &= ~ICANON;
old.c_lflag &= ~ECHO;
old.c_cc[VMIN] = 1;
old.c_cc[VTIME] = 0;
if (tcsetattr(0, TCSANOW, &old) < 0)
	perror("tcsetattr ICANON");
}
//=========================================
void set_winsize()
{
win_size=4;
if(win_size>(int)files.size())
win_size=(int)files.size();
}
void clrscr()
{
cout << "\033[H\033[2J\033[3J" ;

}
void set_position(int x,int y)
{
 //int r=0;
  //  printf("\033[%d;%df",c,r);
  cout<<"\033["<<x<<";"<<y<<"H";

}
//======================================
void print_details(int vecstart,int vecend)
{
clrscr();
set_position(0,0);
int j;
//cout<<"hai";
for(int i=vecstart;i<=vecend;i++)
{
vector<string> tem=files_info[i];
printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",tem[0].c_str(),tem[1].c_str(),tem[2].c_str(),tem[3].c_str(),tem[4].c_str(),tem[5].c_str());
//printvec(i);
}
set_position(10,0);
cout<<"home:"<<string(rootp)<<endl;
set_position(11,0);
cout<<"currdirectory:"<<string(path)<<endl;
}
/*void printvec(int i)
{
vector<string> tem=files_info[i];
printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",tem[0].c_str(),tem[1].c_str(),tem[2].c_str(),tem[3].c_str(),tem[4].c_str(),tem[5].c_str());
}*/
//===============================================
void enable_scroll(int top,int bottom,int vecstart,int vecend)
{
cursor_pos=1;
set_position(1,0);
char c=' ';
while(c!='q')
{
c=cin.get();
switch(c)
{
case 66:{/*scrolldown*/

if(cursor_pos<win_size&&cursor_pos<=vecend){
cursor_pos=cursor_pos+1;
set_position(cursor_pos,0);}
else if(bottom==vecend){
}
else{
top++;
bottom++;
print_details(top,bottom);
set_position(cursor_pos,0);
}
break;
}
case 65:{/*scroll up*/

if(cursor_pos>1)
{cursor_pos=cursor_pos-1;
set_position(cursor_pos,0);}
else if(top==vecstart){}
else{
top--;
bottom--;
print_details(top,bottom);
set_position(cursor_pos,0);
}break;
}
case 107:{/*scrollup k*/
if(top-win_size>0)top=top-win_size;
else top=0;
bottom=top+win_size-1;
print_details(top,bottom);
set_position(cursor_pos,0);
break;
}
case 108:{/*scrolldown l*/
if(vecend>(bottom+win_size))bottom=bottom+win_size;
else bottom=vecend;
top=bottom-win_size+1;
print_details(top,bottom);
set_position(cursor_pos,0);
break;
}
case 68:{/*move back (<-)left arrow*/
if(stackback.size()!=0){
string p=stackback.top();
stackback.pop();
string present=path;
stackfront.push(present);
//char prev[1256];
//strcpy(prev, p.c_str());
//strcpy(f, currpath.c_str());
to_normal_mode(p);}
break;}
case 67:{/*move forward*/
if(stackfront.size()!=0)
{string n;
n=stackfront.top();
stackfront.pop();
string present=path;
stackback.push(present);
//char next[1256]
//strcpy(next,n.c_str());
to_normal_mode(n);}
break;}
case 104:{/*home*/
if(strcmp(path,rootp)!=0)
{
stackback.push(rootp);
to_normal_mode(rootp);
}
break;}
case 127:{/*level up*/
if(strcmp(path,rootp)!=0)
{
string s=path;
int x;
//char *c=strrchr(s, '/');
//x=c - string;
for(int i=0;i<s.length();i++)
{
if(s[i]=='/')x=i;
}
string strin=s.substr(0,x);
stackback.push(strin);
to_normal_mode(strin);
}
break;
}
case 10:{/*enter*/
//cout<<"entered";
string file=files[cursor_pos+top-1];
struct stat st1;
char f1[2056];
string s2=path;
string str;
str=s2+"/"+file;
strcpy(f1, str.c_str());
if(stat(f1,&st1)==0)
{
if(S_ISDIR(st1.st_mode))
{

if(file=="."){}
else if(file=="..")
{
if(strcmp(path,rootp)!=0)
{
string s=path;
int x;
//char *c=strrchr(s, '/');
//x=c - string;
for(int i=0;i<s.length();i++)
{
if(s[i]=='/')x=i;
}
string strin=s.substr(0,x);
//cout<<strin;
stackback.push(strin);
to_normal_mode(strin);
}
}
else{
stackback.push(path);
string s=path;
s=s+"/"+file;
strcpy(path,s.c_str());
to_normal_mode(s);
}
}
else
{
//char f2[2056]=f1.c_str();
pid_t p=fork();
if(p==0)
{//execlp("vi","vi",f1,NULL);
//exit(1);
execl("/usr/bin/xdg-open","xdg-open",f1,NULL);
exit(1);
}
}}
break;}
case ':':{/*command mode*/
to_command_mode();
break;
}
}
}
if(c=='q')quitp();
}
//===================
/*void level_up()
{
if(path==rootp){
   //printAlertLine("You're already present in the home directory");
   return;
        }
else 
{
stackback.push(path);
to_normal_mode("/home");
}
return;
}*/
/////command mode====================================================


///=========================================
string getinput(){
char ch; string command; 
int cursorLoc = 0; 
set_position(20, 0);
while(true){
//clearline();
ch = cin.get();
if(ch==10){
cursorLoc = 0;
set_position(20, cursorLoc);
return command;
}
if(ch==27){
//printAlertLine("Welcome back to Normal Mode                                      ");
return "esc";
}
else{

if(ch==127){
if(cursorLoc==0) continue;
command.pop_back();
--cursorLoc;
set_position(20,0);
clearline();
cout<<command;
set_position(20,command.size()+1);
}
else{
command.push_back(ch);
set_position(20,0);
clearline();
cout<<command;
++cursorLoc;
set_position(20, cursorLoc+1);
}
}
}
return " ";
}

//============================================
void parse_command(string command)
{ 
/*int j=0;
for(int i=0;i<command.size();i++)
{
if(command[i]==' ')
{
string s=command.substr(j,i-1);
j=i+1;
com.push_back(s);
}
}
for(auto s:com)
cout<<s<<endl;*/
stringstream ccommand(command);
string s;
while(getline(ccommand, s, ' ')) 
com.push_back(s);
}
///==================================change path==========================
int change_path()
{
int x=com.size();
string p=com[x-1];
if(p[0]=='.')
{
p=path;
com[x-1]=p;
}
else if(p[0]=='/')
{p=path+p;
com[x-1]=p;}
else if(p[0]=='~')
{p.replace(0,1, rootp);
com[x-1]=p;}
else return 0;
//cout<<p;
return 1;
}
//=====================================

void perform()
{
int t=change_path();
string s4=com[0];
set_position(22,0);
clearline();
if(!t && s4!="search" && s4!="rename")
cout<<"invalid address";
else{
//string q=com[0];
if(com[0]=="goto")
{
stackback.push(path);
fun_goto();
cout<<"sucessful";
}
else if(com[0]=="search")
{
//cout<<com[0];
int x=fun_search(path,com[1]);
//set_position(23,0);
if(x==1)
cout<<"file found";
else if(x==0) cout<<"file not found";
}
else if(com[0]=="rename")
{
fun_rename();
cout<<"sucessful";
}
else if(com[0]=="create_file")
{
fun_createfile();
cout<<"sucessful";
}
else if(com[0]=="create_dir")
{
fun_createdir();
cout<<"sucessful";
}
else if(com[0]=="copy")
{fun_copy();
cout<<"sucessful";
}
else if(com[0]=="delete_file")
{
int x1=deletefile(com[1]);
if(x1==0)cout<<"deletion done";
else cout<<"deletion not done";
}
else if(com[0]=="delete_dir")
{
string del=path;
if(del==com[1])
cout<<"we should not delete current directory";
else 
{
deletedir(com[1]);
rmdir(com[1].c_str());
cout<<"done sucessfully";
}
}
else if(com[0]=="move")
{
fun_move();
cout<<"sucessful";
}
else cout<<"wrong command";
}
set_position(25,0);
clearline();
string s7=path;
cout<<"current path::"<<s7;

}
////====================================
void to_command_mode()
{
set_position(18,0);
cout<<"************command mode************";
set_position(20,0);
while(true)
{
string command=getinput();
com.clear();

if(command=="esc"){
to_normal_mode(rootp);
//break;
}
else 
{

//set_position(22,0);

//clearline();
//cout<<"enter";
//set_position(22,0);
//clearline();
//set_position(23,0);
//clearline();
//set_position(26,0);
 parse_command(command);
 perform();
}
}
}
////////=======================goto=================
void fun_goto()
{
chdir(com[1].c_str());
string s=com[1];
//string p=path;
//p=p+"/"+com[1];
strcpy(path,s.c_str());
//to_normal_mode(com[1]);
//cout<<s<<" "<<path;
}
//=============serach
int fun_search(string dirname,string filename)
{

//set_position(29,0);
//clearline();
//cout<<filename;
vector<string> fil;
DIR *dir; 
struct dirent *diread;
char f[256];
strcpy(f, dirname.c_str());
if ((dir = opendir(f)) != nullptr) {
	 while ((diread = readdir(dir)) != nullptr) {
		//fil.push_back(diread->d_name);
		string s=diread->d_name;
		if(s=="."||s=="..")continue;
		else if(s==filename){return 1;}
		else {
		struct stat s1;
		if(stat(diread->d_name,&s1)==0){
		if(S_ISDIR(s1.st_mode))
		{
		int y=fun_search(dirname+"/"+s,filename);
		if(y==1)return 1;
		}
 		}}}
        closedir (dir);
    } else {
        cout<<"cannot open dir";}
      
return 0;
}
//=========================rename==================
void fun_rename(){
       if(com[0]=="rename"){
	string old1=com[1];
	string new1= com[2];
	//set_position(27,0);
	//cout<<path;
	chdir(path);
	rename(old1.c_str(),new1.c_str());
	//set_position(25,0);
	//cout<<"done rename";
	}
}
//===============================create==========================
void fun_createdir()
{
int x=com.size();
string p=com[x-1];
for(int i=1;i<x-1;i++)
{
string dirname=com[i];
string y=p+"/"+dirname;
mkdir(y.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
}
}

void fun_createfile()
{
int x=com.size();
string p=com[x-1];
for(int i=1;i<x-1;i++)
{
string filename=com[i];
string y=p+"/"+filename;
open(y.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
}
}
//=======================copy===========================

void fun_copy()
{
int x=com.size();

for(int i=1;i<x-1;i++)
{
string f=com[i];
string s=path;
s=s+"/"+f;
string p=com[x-1];
struct stat s3;
if(stat(s.c_str(),&s3)==0)
{
if(S_ISDIR(s3.st_mode))
{
string d=p+"/"+f;
mkdir(d.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
copydir(s,d);
}
else 
{
p=p+"/"+f;
copyfile(f,p);
}
}
}
}
//*****************
void copyfile(string filename,string dest)
{
char buffer[1024];
int finput,foutput, nread;
finput = open(filename.c_str(),O_RDONLY);
foutput = open(dest.c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
while((nread = read(finput,buffer,sizeof(buffer)))>0){
write(foutput,buffer,nread);
//chdir(path);
}

}
//********************8
void copydir(string dirname,string dest)
{
DIR *dir; 
struct dirent *diread;
char f[1256];
strcpy(f, dirname.c_str());
if ((dir = opendir(f)) != nullptr) {
       chdir(f);
	 while ((diread = readdir(dir)) != nullptr) {
		//fil.push_back(diread->d_name);
		//set_position(26,0);
		//cout<<dirname;
		string s=diread->d_name;
		struct stat s1;
		string d=dest+"/"+s;
		if(stat(diread->d_name,&s1)==0){
		if(S_ISDIR(s1.st_mode)){
		if(s=="."||s=="..")continue;
		else {
		
		mkdir(d.c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
		copydir(dirname+"/"+s,d);
		}}
		else{
		copyfile(s,d);}}}
		chdir("..");
 		
        closedir (dir);
    } else {
        cout<<"cannot open directory";}

}
//=========================delete====================
int deletefile(string file)
{
int x=unlink(file.c_str());
return x;
}
//*******************
void deletedir(string dest)
{
DIR *dir; 
//set_position(28,0);
//cout<<path<<"    "<<dest;
struct dirent *diread;
char f[1256];
strcpy(f, dest.c_str());
if ((dir = opendir(f)) != nullptr) {
       chdir(f);
	 while ((diread = readdir(dir)) != nullptr) {
		//fil.push_back(diread->d_name);
		
		
		string s=diread->d_name;
		struct stat s1;
		
		if(stat(diread->d_name,&s1)==0){
		if(S_ISDIR(s1.st_mode)){
		if(s=="."||s=="..")continue;
		else {
		deletedir(diread->d_name);
		rmdir(diread->d_name);
		}}
		else{
		unlink(diread->d_name);}}}
		chdir("..");
 		
        closedir (dir);
    } else {
        cout<<"cannot open directory";}

}
//=================move==============
void fun_move()
{
fun_copy();
int x=com.size();
for(int i=1;i<x-1;i++)
{
string f=com[i];
string s=path;
s=s+"/"+f;
string p=com[x-1];
struct stat s3;
if(stat(s.c_str(),&s3)==0)
{
if(S_ISDIR(s3.st_mode))
{
deletedir(com[i]);
rmdir(com[i].c_str());
//cout<<"done sucessfully";
}
else 
{
deletefile(com[i]);
}
}
}
}
//******************************
void quitp()
{
clrscr();
old.c_lflag |= ECHO;
    //keeping icanon flag
    old.c_lflag |= ICANON;
    //restoring terminal state
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
    {   // checking for error
        perror ("tcsetattr ~ICANON");
    }
    
    exit(0);
}
//=================================
int main() {
    //char tmp[256];
    getcwd(path, 1256);
    getcwd(rootp, 1256);
   // cout << "Current working directory: " << tmp << endl;
   //string p1="/home";
   //strcpy(path,p1.c_str());
   stackback.push(path);
   to_rawmode();
  to_normal_mode(stackback.top());
  //while(1){}
  //to_normal_mode(path);
//cout<<stackback.top();

to_command_mode();
quitp();
return 0;
}
