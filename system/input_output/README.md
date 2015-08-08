__Unix I/O__

The goal of this exercise is to practice UNIX I/O system calls and to see the difference in performance between C++ library functions and system calls. My implementation consist of the following:

* A function that uses the C++ library get() and put() function to copy from an input file to the output file one character at a time.
* A function that uses the UNIX system calls read() and write() to copy the input file to the output file one character at a time.
* A function that uses the UNIX system calls read() and write() to copy the input file to the output file one buffer at a time (buffer size is "BUFSIZ" as declared in \<stdio.h>).

The three methods of copying a file runs N time each to test the difference between performance (N = 4 in this case). The Timer class measures the performance of each of the three approaches to copying a file. The input file use is linux.words.
