#include "du_scanner.h"

// referred from vivian's code
int iterate_dir(char *dirpath)
{
    DIR *pDir; //定义一个DIR类的指针
    struct dirent *ent;
    verbose_msg("Scanning DIR: %s", dirpath);
    pDir = opendir(dirpath);
    if (NULL == pDir)
    {
        fprintf(stderr, "PATH : %s not exist,exit this function\n", dirpath);
        return -1;
    }
    while ((ent = readdir(pDir)) != NULL) //读取pDir打开的目录，并赋值给ent, 同时判断是否目录为空，不为空则执行循环体
    {
        if (ent->d_type & DT_DIR) //读取 打开目录的文件类型 并与 DT_DIR进行位与运算操作，即如果读取的d_type类型为DT_DIR (=4 表示读取的为目录)
        {

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                //如果读取的d_name为 . 或者.. 表示读取的是当前目录符和上一目录符, 用contiue跳过，不进行下面的输出
                continue;
            char *childpath = (char *)malloc(sizeof(char) * (strlen(dirpath) + MAXNAMLEN + 1));
            sprintf(childpath, "%s%s/", dirpath, ent->d_name); //如果非. ..则将 路径 和 文件名d_name 付给childpath, 并在下一行prinf输出
            verbose_msg("New Path: %s", childpath);
            iterate_dir(childpath); // recursion
            free(childpath);
        }
        else //如果读取的d_type类型不是 DT_DIR, 即读取的不是目录，而是文件，则直接输出 d_name, 即输出文件名
        {
            char *filepath = malloc(sizeof(char) * (strlen(dirpath) + strlen(ent->d_name) + 2));
            sprintf(filepath, "%s%s", dirpath, ent->d_name);
            compare(filepath);
        }
    }
    closedir(pDir);

    return 0;
}