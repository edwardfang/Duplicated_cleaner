# Duplicated_cleaner

This is a collaborative lab assignment for CS205 in SUSTech

 - Programming Language C (enough)
 - Due: 14:00, July. 15

## Structure

 - dufinder.c deal with input arguments
     - Display Usage: when abort due to error input
     - Input dir
     - debug arg
     - verbose arg more infomation
     - example ./cleaner mydir [-v] [-d]
 - file_enumerate.c enumerate all the files
     - recursion
     - duplicated files => stdout
     - -d debug info => stderr
     - -v current scanning dir; file details: size, type, hash => stderr
     - outo f memory detection
 - comparision.c compare two files
     - check file size
     - check first and last range of bytes (note the total size of file)
     - check file type
     - check hash: ref: https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
 - Makefile