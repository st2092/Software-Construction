#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

const int MAX_COMMAND_LENGTH = 256;
const int MAX_ARRAY_SIZE = 1024;  
bool background_task = false;
bool piping_required = false;
// io redirection flags
bool in = false;
bool out = false;
int input; // for open and read from
int output; // for open and write to
//child pid variable
int child_pids [100];
int child_pid_index = 0;
int current_child_pid;

FILE* pipe_fp; //for piping purposes
bool piping = false;
///////////////////////////////////////////////////////////
// interrupt handler(s)
///////////////////////////////////////////////////////////
void
ctrl_c_handler ( int signum )
{
  //after catching ctrl c interrupt, use the kill command to redirect to the child process
  cout << "SIGINT signal caught." << endl;
  kill (current_child_pid, SIGINT);
}

///////////////////////////////////////////////////////////
// child reaper
///////////////////////////////////////////////////////////
void
child_reaper (int ignore)
{
  int status;
  signal (SIGCHLD, child_reaper);
  while ( waitpid(-1, &status, WNOHANG) >= 0 )
    continue;
}

/////////////////////////////////////////////////////////////
// Main shell related functions and helper functions
/////////////////////////////////////////////////////////////
/* helper function to zero out argv */
void
reset_argv (char* argv[])
{
  for (int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    argv[i] = '\0';
  }
  return;
}

/* helper function to reset c string */
void
reset_array (char* arr)
{
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++)
    {
      arr[i] = '\0';
    }
}

/* helper function to copy content of one array to another */
void
copy (char src[], char dst[])
{
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++)
    {
      dst[i] = src[i];
    }
}

/* function to determine if task is a background task */
bool
is_background_task (char* & command_line)
{
  string command_line_str (command_line);
  // if the command line contains the char '&' then it is a background task
  if (command_line_str.find ("&") != string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* IO redirection function */
void
io_redirection ( bool & in, bool & out, char* token, int & input, int & output)
{
  if (*token == '<')
  {
    in = true;
    input = open ( strtok(NULL, " \n"), O_RDONLY ); //open the file afterwards as read only
  }
  else if (*token == '>')
  {
    out = true;
    //open the following file for writing, if not existent then create, truncate, etc...
    output = open ( strtok(NULL, " \n"), O_WRONLY | O_CREAT | O_TRUNC, S_IWGRP | S_IRUSR | S_IWUSR);
  }
  return;
}

/*
  Parse command line function
  - Takes in the entire command line and parse through it
  - sets up for piping, io redirection, and background tasks
*/
void
parse_command_line (char* command_line, char* argv[])
{
  reset_argv (argv);
  char* token;
  //check if command is a background task
  if (is_background_task(command_line))
  {
    background_task = true;
  }
  else
  {
    background_task = false;
  }
  //begin parsing command line and setting up argv for execution
  argv[0] = strtok(command_line, " \n"); // command
  int i = 1;
  //traverse through the command line searching for piping
  for (; (token = strtok(NULL, " \n")) && *token != '<' && *token != '>'; i++)
  {
    if (*token == '|')
    {
      piping = true;
    }
    else if ( *token != '&' )
    {
      argv[i] = token;
    }
  }
  // end of the command line, now to handle for io redirection
  argv[i] = NULL;
  while (token)
  {
    if (!piping)
    {
      //io redirection
      io_redirection (in, out, token, input, output);
    }
    token = strtok (NULL, " \n");
    if (!piping && token && *token == '|')
    {
      piping = true;
    }
  }
}

/* locate command function which creates the path to command  */
char* locate_command (char* command)
{
  char* current_created_path;
  //get PATH environment variable
  char* path = getenv("PATH");
  struct stat buf;
  // We want to remove ':' from PATH and then concatenate it with the following path until ':'.
  // This process is repeated until stat finds a valid path
  current_created_path = strtok(path, ":");
  //use a temp char array to create the path
  char temp_arr[MAX_COMMAND_LENGTH] = {'\0'};
  //traverse through all possible paths looking for a valid path to the command
  while (current_created_path = strtok(NULL, ":"))
  {
    //strcat copies the current path to temp_arr
    strcat (temp_arr, current_created_path);
    //append '/' with the command and then check with stat if path is valid
    strncat (temp_arr, "/", 1);
    strncat (temp_arr, command, MAX_COMMAND_LENGTH);
    if ( stat(temp_arr, &buf) == 0 )
    {
      char* valid_path = temp_arr;
      return valid_path;
    }
    reset_array(temp_arr); //clear temporary array if created path was not a valid one
  }
  return NULL; //if no valid path can be found return NULL
}

/* piping process */
void
pipe_process (char* command_line)
{
    if ( (pipe_fp = popen(command_line, "w")) == NULL )
    {
      perror ("popen error");
      exit (-1);
    }
}

/* grandchild process action */
void
grandchild_process_action (char* command_line, char* argv[], char* command)
{
  switch (current_child_pid = fork())
  {
    case -1:
      perror ("grandchild fork error");
      exit (-1);
      break;
    case 0: //grandchild
      signal (SIGINT, SIG_IGN);
      if (piping)
      {
        pipe_process (command_line);
        pclose(pipe_fp);
        exit(0);
      }
      else //io redirection or just a simple command line
      {
        if (in)
        {
          dup2(input, 0);
          close(input);
        }
        if (out)
        {
          dup2(output,1);
          close(output);
        }
        command = locate_command(argv[0]);
        //if command was found
        if (command)
        {
          if (execv(command, argv) == -1)
          {
            perror ("grandchild execv error");
            exit(-1);
          }
        }
        exit(0);
      }
      break;
    default: //child
      exit(0);
      break;
  }
}

/* child process actions */
void
child_process_action ( char* command_line, char* argv[], char* command )
{
  //if piping is involve
  if (piping)
  {
    pipe_process ( command_line );
    pclose(pipe_fp);
    exit(0);
  }
  // no piping and is just a background task
  else if (background_task)
  {
    grandchild_process_action (command_line, argv, command);
  }
  //io redirection 
  else
  {
    if (in)
    {
      dup2(input, 0);
      close(input);
    }
    if (out)
    {
      dup2(output, 1);
      close(output);
    }
    command = locate_command(argv[0]);
    //if the command was found
    if (command)
    {
      //call execv and run the command
      if (execv(command, argv) == -1)
      {
        perror ("execv error");
        exit(-1);
      }
    }
    exit(0);
  }
}

/* function to execute the command */
void
execute_command ( char* command_line, char* argv[], char* command )
{
  switch ( current_child_pid = fork() )
  {
    case -1: //error
      perror("fork error");
      exit(-1);
      break;
    case 0: //child process
      child_process_action (command_line, argv, command);
      break;
    default: //parent
      // if child process is a background one add child's pid to list
      if (background_task)
      {
        child_pids[child_pid_index] = current_child_pid;
        child_pid_index++;
      }
      break;
  }
}

/* End all background tasks function */
void
end_background_tasks ()
{
  for (int i = 0; i < child_pid_index; i++)
  {
    kill (child_pids[i] + 1, SIGKILL); //we add one since grandchild is 1 after child
  }
}

int main ()
{
  char* command;
  char* argv[MAX_ARRAY_SIZE];
  int status;
  
  // set up interrupt catcher
  struct sigaction ctrl_action;
  ctrl_action.sa_handler = ctrl_c_handler;
  sigemptyset (&ctrl_action.sa_mask);
  ctrl_action.sa_flags = SA_RESTART;
  
  //initialize catcher to catch ctrl+c signal
  if (sigaction(SIGINT, &ctrl_action, NULL) == -1)
  {
    perror ("CTRL + C error");
    exit(-1);
  }
  
  //initialize child signal catcher
  signal (SIGCHLD, child_reaper);
  
  //start shell
  while (1)
  {
    //reset boolean flags
    background_task = false;
    in = false;
    out = false;
    piping = false;
    
    cout << "% ";
    char command_line[MAX_COMMAND_LENGTH];
    char command_line_for_piping[MAX_COMMAND_LENGTH];
    fgets (command_line, MAX_COMMAND_LENGTH, stdin);
    
    copy (command_line, command_line_for_piping);
    if ( strcmp (command_line, "exit\n") == 0 || strcmp (command_line, "quit\n") == 0)
    {
      end_background_tasks();
      break; //exit the while loop/shell
    }
    //parse command line
    parse_command_line(command_line, argv);
    //set the command
    command = argv[0];
    //execute the command
    execute_command (command_line_for_piping, argv, command);
    //if the command is a foreground task, we wait until child is done
    if (!background_task)
    {
      wait (&status);
    }
  }
  return 0;
}
