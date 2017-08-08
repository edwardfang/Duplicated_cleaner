# Duplicated_cleaner
This is a collaborative lab assignment for CS205 in SUSTech


## Structure

 - dufinder.c deal with input arguments
     - Display Usage: when abort due to error input
     - Input dir
     - debug arg
     - verbose arg more infomation
 - file_enumerate.c enumerate all the files
     - recursion
     - duplicated files => stdout
     - -d debug info = stderr
     - -v current scanning dir; file details: size, type, hash => stderr
 - comparision.c compare two files
     - check file size
     - check first and last range of bytes (note the total size of file)
     - check file type
     - check hash: ref: https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
 - Makefile