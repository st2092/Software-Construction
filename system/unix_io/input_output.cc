#include "Timer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
using namespace std;

//////////////////////////////////////
// function: get and put test
//////////////////////////////////////
// Uses the C++ library functions (get(char) and put(char)) to copy input file to output file
// one character at a time
//////////////////////////////////////
void
get_and_put_test(const string & input, const string & output)
{
  //open the file we want to copy to the output file
  ifstream in(input.c_str());
  if ( !in.good() )
  {
    cerr << "Error opening " << input << endl;
    exit(-1);
  }
  //open the file that will get copy into
  ofstream out(output.c_str());
  if ( !out.good() )
  {
    cerr << "Error opening " << output << endl;
    exit(-1);
  }
  
  //copy one character at a time to output file
  while ( in.good() )
  {
    char c = in.get();
    out.put(c);
  }
  in.close();
  out.close();
}

//////////////////////////////////////
// function: read and write one test
//////////////////////////////////////
// Uses the Unix read and write function to write one character at a time
//////////////////////////////////////
void
read_and_write_one_test(const string & input, const string & output)
{
  int file_des = open(input.c_str(), O_RDONLY); //read only
  if (file_des == -1)
  {
    cerr << "Error opening " << input << endl;
    exit(-1);
  }
  int file_des2 = open(output.c_str(), O_WRONLY | O_CREAT | O_TRUNC); //write, create, truncate
  if (file_des2 == -1)
  {
    cerr << "Error opening " << output << endl;
    exit(-1);
  }
  
  char data[1];
  while( read(file_des, data, 1) )
  {
    if ( !write(file_des, data, 1) ) //write to output file failed
    {
        cerr << "Error writing (1 by 1) to " << output << endl;
        exit(-1);
    }
  }
  //close the input and output file
  close(file_des);
  close(file_des2);
}

//////////////////////////////////////
// function: read and write buf test
//////////////////////////////////////
// Uses the Unix read and write function to write one full BUFSIZ at a time
//////////////////////////////////////
void
read_and_write_buf_test(const string & input, const string & output)
{
  int file_des = open(input.c_str(), O_RDONLY); //read only
  if (file_des == -1)
  {
    cerr << "Error opening " << input << endl;
    exit(-1);
  }
  int file_des2 = open(output.c_str(), O_WRONLY | O_CREAT | O_TRUNC); //write, create, truncate
  if (file_des2 == -1)
  {
    cerr << "Error opening " << output << endl;
    exit(-1);
  }
  
  char data[BUFSIZ];
  while ( read(file_des, data, BUFSIZ) )
  {
    if ( !write(file_des2, data, BUFSIZ) ) //write to output file failed
    {
      cerr << "Error writing (by BUFSIZ) to " << output << endl;
      exit(-1);
    }
  }
  //close the input and output file
  close(file_des);
  close(file_des2);
}

///////////////////////////////////////////
// function: print status
///////////////////////////////////////////
// Prints the time elapsed for a task to complete
///////////////////////////////////////////
void
print_status(Timer & timer, double & etime)
{
  timer.elapsedWallclockTime(etime);
  cout << "Wall Clock time: " << etime << endl;
  timer.elapsedUserTime(etime);
  cout << "User time: " << etime << endl;
  timer.elapsedSystemTime(etime);
  cout << "System time: " << etime << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 3 )
  {
    cerr << "Incorrect format. The command line should follow: <program_name> <input_file> <output_file>" << endl;
    exit(-1);
  }
  Timer timer;
  double elapsed_time;
  int n = 4; //indicates how many times to run each test

  ////////////////////////////////////////////////////////
  //get and put test
  ////////////////////////////////////////////////////////
  cout << "Get and put function test starting..." << endl;
  timer.start();
  for (int i = 0; i < n; i++)
  {
    get_and_put_test(argv[1], argv[2]);
  }
  //show status of how long the process take to run
  print_status(timer, elapsed_time);
  
  //////////////////////////////////////////////////////
  //read and write one char at a time test
  //////////////////////////////////////////////////////
  cout << "\nRead and Write one character at a time test starting..." << endl;
  timer.start();
  for (int i = 0; i < n; i++)
  {
    read_and_write_one_test(argv[1], argv[2]);
  }
  print_status(timer, elapsed_time);
  
  //////////////////////////////////////////////////////
  //read and write one full buffer size at a time test
  //////////////////////////////////////////////////////
  cout << "\nRead and Write one buffer at a time test starting..." << endl;
  timer.start();
  for(int i = 0; i < n; i++)
  {
    read_and_write_buf_test(argv[1], argv[2]);
  }
  print_status(timer, elapsed_time);
  
  return 0;
}
