# Duplicated_cleaner

This is a collaborative lab assignment for CS205 in SUSTech

 - Programming Language C (enough)
 - Group member: Yidong FANG 11510493, Yi XIE 11510074, Jiaqi ZHANG 11611907
 - Due: 14:00, July. 15

## How to run it

 - First run `make`
 - Then just run `./build/duscanner dir [-v] [-d]`
 - Memory limit of the program can be set by the `MEMORY_LIMIT` in `du_scanner.h` 

## Structure

 - du_scanner.c entry for the program
     - Display Usage: when abort due to error input
     - Input dir
     - debug arg
     - verbose arg more infomation
     - example ./cleaner mydir [-v] [-d]
     - debug/info msg display function
     - -d debug info => stderr
     - -v current scanning dir; file details: size, type, hash => stderr
 - file_iterator.c tranverse all the files in the directory
     - recursion
     - dynamically allocated memory for the filepath
 - file_compare.c
     - check file read permission
     - get the subpath
     - duplicated files => stdout
     - auto out of memory detection
     - check file size
     - check beginning, ending, and random block in large file
     - check file type
     - check hash(actully fast only when dealing with multiple same big files) ref: https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
 - data_structure.c
     - used to store the file info in a good data stucture for fast retriving
     - banlanced tree from GNU
     - self-defined linked list
     - self-defined file info structure
 - getRSS.c
     - get memory usage info from the system
     - ref: http://NadeauSoftware.com/
 - Makefile
     - fast debug, just run `make debug`
     - advanced `wildcards`, `patsubst`, etc.