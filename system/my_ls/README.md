__My ls__

The goal of this implementation is to have the program behave like the Unix ls command. More specifically, the program is to replicate a subset of 'ls'. The program lists in ascending alphabetical order one name per line indented by the nesting depth of the directory. If there are no arguments to the program, then it lists the files in ".". Otherwise, for each command-line argument N, the following is perform:
* if N is a directory, print the name of the directory, then indent one tab and do my_ls of each entry in N (i.e., like ls -R)
* if N is a file, print the following information for file N in the format (ls -lg): octal_mode num_links size_in_bytes mod_date name
