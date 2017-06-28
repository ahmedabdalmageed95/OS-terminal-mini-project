//Mohamed Khamis Gaber		1132143
//Ahmed Abd El-Magid		1135116

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

void handler(int signum);

/*this function is used to get current working directory by geting username, machine name and currnet path then concatinate*/
string getname(string path)
{
	char * writable = new char[path.size() + 1];
    copy(path.begin(), path.end(), writable);
    char y[1024];
    int q=gethostname(y,sizeof(y));
	string usr;
	struct passwd *pw=getpwuid(geteuid());
	usr=pw->pw_dir;
    usr.erase(0,6);
    string z;
    string twodots=":";
    string dollar="$";
    string at="@";
    z=usr+at+y+twodots+path+dollar;
    return z;

}


int main()
{
	string pth;
	pth=get_current_dir_name(); // set the default path
	int status;
	signal(SIGINT,handler); // this signal for ctrl+c command
	while(1)
	{
		//int*status=new int ;
 		string z=getname(pth);
 		cout<<z<<" "; //display username+machinename+path
 		vector<char*>v;
		bool bck=0;
		string str;
		getline(cin,str); //reading string 
        string exiter=""; 

        // this loop removes spaces from input string(str) then puts them in a string exiter
        for (int i=0;i<str.size();i++)
		{
			if (str[i]!=' ')
				{
					exiter+=str[i];
				}
		}

        if (exiter=="exit")
		{      
			break; //check if exit is enter terminate the program
		}

		else if (exiter.size()==0)
		{
			continue; //this handels the case if empty string is entered
		}

        else 
        {
			if(exiter[0]=='c'&&exiter[1]=='d'&&exiter[2]!=str[2]) // check if entered strings are cd to exec cd command and also check that what after cd is a space
 			{
 				string tmp="";
 				for (int i=2;i<str.size();i++) // continue after cd so start from i=2
 				{
 					if (exiter[i]!=' ') // place the input string in a substring called temp
 					{
 						tmp+=exiter[i];
 					}
 				}
 				const char*path=new char[tmp.size()+1]; // an array of characters equal to the size of the tmp substrings
 				path=tmp.c_str(); // path now holds what is enterd after cd command as characters
   				int w=chdir(path);
   				if (w<0)
   				{
   					cout<<"error"<<endl;
   				}
   				pth="";// clear path
   				pth=getcwd(NULL,255);
 			}

 			else // if what is enterd is not cd but another command we must fork
 			{
				int pid=fork(); // create a child to execut entered command

				if (pid==0) // code for child
				{
					string temp="";
					for (int i=0;i<str.size();i++) //this loop is to divide the string to substrings 
					{
	
						if (str[i]!=' ') // if not entered is a space then add to temp
							temp+=str[i];

						if ((str[i]==' ' || i==str.size()-1)&&temp.size()!=0) // if what is enterd is a space of the last element
						{
							char * writable = new char[temp.size() + 1]; // we create an array of charaters equal to the size of the input string
    						copy(temp.begin(), temp.end(), writable); // we take input in string while functions work with array of charaters so we change from string to charaters
							writable[temp.size()]='\0';	//termnating charcter is set
 							v.push_back(writable); // push the string in a vector of characters
 							temp=""; // empty the input string to allow for another string to be enterd

						}
					}

 					v.push_back(NULL); //the last element of the array of arguments sent to execvp must be NULL
 					char**cont =new char* [v.size()]; // since v is a vector that holds multiple strings in form of characters we create a 2D character array and save it's value to whats in the vector
 					for (int i=0;i<v.size();i++)
 						cont[i]=v[i];

 					char**const tmp=cont; //point to constant
 					string strd(v[0]); // strd string that holds the first input entered which is the required command
                    //string ampcheck(v[v.size()-2]);
                    // check which command is enterd
 					if (strd== "ls")
 					{
 						execvp("/bin/ls",tmp);
 					}
 					else if (strd=="ps")
 					{
 						execvp("/bin/ps",tmp);
 					}
 					else if(strd=="kill")
 					{
 						execvp("/bin/kill",tmp);
 					}
 					else if (strd[0]=='.'&&strd[1]=='/')
 					{
 						const char*path=new char[str.size()+1];
 						path=str.c_str();
 						execvp (path,tmp);
					}
 					else
 					{
 						cout <<"not a supported command"<<endl;
 					}
 			
 				
 				}

 				// code for parent
			    if (pid >0)//)&&bck==0)
                {
					wait(&status);
                }
	
			}
		}
	}

return 0;

}
void handler(int signum)
{

}
