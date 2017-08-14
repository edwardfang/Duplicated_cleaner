#include "cleaner.h"

// referred from vivian's code
int iterate_dir(char *dirpath)
{
    DIR *pDir;          //定义一个DIR类的指针
    struct dirent *ent; //定义一个结构体 dirent的指针，dirent结构体见上
    //int i = 0;
    char childpath[512]; //定义一个字符数组，用来存放读取的路径
    printf("listDir, the input dirpath : %s\n", dirpath);
    pDir = opendir(dirpath);                    //  opendir方法打开path目录，并将地址付给pDir指针
    memset(childpath, 0, sizeof(childpath)); //将字符数组childpath的数组元素全部置零

    if (NULL == pDir)
    {
        printf("PATH : %s  not exist,exit this function",dirpath);
        return -1;
    }
    while ((ent = readdir(pDir)) != NULL) //读取pDir打开的目录，并赋值给ent, 同时判断是否目录为空，不为空则执行循环体
    {
        if (ent->d_type & DT_DIR) //读取 打开目录的文件类型 并与 DT_DIR进行位与运算操作，即如果读取的d_type类型为DT_DIR (=4 表示读取的为目录)
        {

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                //如果读取的d_name为 . 或者.. 表示读取的是当前目录符和上一目录符, 用contiue跳过，不进行下面的输出
                continue;

            sprintf(childpath, "%s/%s", dirpath, ent->d_name); //如果非. ..则将 路径 和 文件名d_name 付给childpath, 并在下一行prinf输出
            printf("path:%s\n", childpath);

            iterate_dir(childpath); //递归读取下层的字目录内容， 因为是递归，所以从外往里逐次输出所有目录（路径+目录名），
                                //然后才在else中由内往外逐次输出所有文件名
        }
        else //如果读取的d_type类型不是 DT_DIR, 即读取的不是目录，而是文件，则直接输出 d_name, 即输出文件名
        {
            printf("File: %s/%s",childpath, ent->d_name);
        }
    }

    return 0;

}