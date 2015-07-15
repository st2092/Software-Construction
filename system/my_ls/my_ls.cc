#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <list>
#include <math.h>
#include <iomanip>
#include <vector>
using namespace std;

const int tab_size = 4;
const int max_size = 256;

//////////////////////////////////
// Function: perform_ls
// Performs behavior similar to that of UNIX ls command.
//////////////////////////////////
int
perform_ls(string directory_name, int indent_depth)
{
  //open directory
  DIR* dirp;
  if ( !(dirp = opendir(directory_name.c_str())) ) //error opening directory
  {
    cerr << "Error (" << errno << ") opening " << directory_name << endl;
    return errno;
  }
  
  /*
  * After directory is successfully opened, we read in what is in the directory
  * and perform the appropriate actions.
  */
  
  list<string> directories;
  vector<string> dir_names;
  dirent* direntp;
  //traverse through the directory until nothing is left
  while( direntp = readdir(dirp) )
  {
    string path = direntp->d_name; //name of current object
    string current_directory_name = direntp->d_name;
    
    if ( path != "." && path != "..")
    {
      //create full path
      path = directory_name + "/" + path;
    }
    if(path == "." && indent_depth == 0)
    {
      cout << ".:" << endl;
    }
    
    //find the specified directory from path
    struct stat buf;
    if ( stat(path.c_str(), &buf) == -1 )
    {
      cerr << "There was an error finding " << direntp->d_name << endl;
      return errno;
    }
    
    //this path leads to a regular file
    //for a regular file we want to display info of the file similar to that of UNIX ls command
    if ( S_ISREG(buf.st_mode) )
    {
      cout << string(indent_depth, ' ') << oct << buf.st_mode << " " << buf.st_nlink
           << " " << dec << buf.st_size;
      //display last modified time
      time_t t = buf.st_mtime;
      struct tm* info_time;
      char buff[max_size];
      info_time = localtime(&t);
      strftime(buff, max_size, "%b %d %H:%M", info_time); //write string version for time format
      cout << "\t" << buff << " " << direntp->d_name << endl;
    }
    //this path leads to a directory
    else if( S_ISDIR(buf.st_mode) )
    {
      if (path != "." && path != "..")
      {
        //add to list of directories to recursively explore later
        directories.push_back(directory_name + "/" + current_directory_name);
        cout << string(indent_depth, ' ') << oct << buf.st_mode << " " << buf.st_nlink
             << " " << dec << buf.st_size;
        //display last modified time
        time_t t = buf.st_mtime;
        struct tm* info_time;
        char buff[max_size];
        info_time = localtime(&t);
        strftime(buff, max_size, "%b %d %H:%M", info_time); //write string version for time format
        cout << "\t" << buff << " " << direntp->d_name << "/" << endl;
      }
    }
  }
  closedir(dirp); //close current directory we are in
  
  //traverse through directory list and repeat the same process as above
  //this process ends when there are no more subdirectories 
  list<string>::iterator i;
  for (i = directories.begin(); i != directories.end(); ++i)
  {
    cout << string(indent_depth + tab_size, ' ') << *i << endl;
    perform_ls(*i, indent_depth + tab_size);
  }
  return 0;
}


int main(int argc, char* argv[])
{
  if(argc > 1) // ls only for the specified directories by user
  {
    for (int i = 1; i < argc; i++)
    {
      perform_ls(argv[i], 0);
    }
  }
  else // ls at current directory
  {
    perform_ls(".", 0);
  }
  return 0;
}
