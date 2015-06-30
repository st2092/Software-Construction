A basic implementation of the UNIX bash shell. Some of the features of this implemenation are:
* adds background jobs when the command ends with '&'
* CTRL + C (^C) does not exit the shell, but the foreground job do get interrupted when the user enters ^C
* pipelines of commands specified with "|" between commands

__How to run shell implementation__

1. Compile the shell cc file with g++. Example: g++ \<file_name>.cc -o shell
2. If there are no errors with compiling, there will be a new executable file called "shell" in the same directory as the cc file.
3. To run this executable enter the following command onto the terminal: "./ shell"
4. At this point, the shell is running and you can enter command lines as you would on bash. 

Here are some sample commands this implemenation can handle:
* "tee newOutputFile > newOutputFile < existingInputFile &"
* "cat < existingInputFile | tee newOutputFile > newOutputFile"
* "cat < existingInputFile | tee newOutputFile > newOutputFile &"
* "cat < existingInputFile | tr A-Z a-z | tee newOutputFile | tr a-z A-Z > newOutputFile"
* "cat < existingInputFile | tr A-Z a-z | tee newOutputFile | tr a-z A-Z > newOutputFile &"
