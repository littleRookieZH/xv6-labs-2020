#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path, char *buf)
{
    char *p;
    for (p = path + strlen(path); (p >= path) && (*p != '/'); --p)
        ;
    ++p;
    if (strlen(p) > DIRSIZ)
    {
        return buf;
    }
    memmove(buf, p, strlen(p));
    return buf;
}

void matchFile(char *path, char *name)
{
    char buf[512];
    memset(buf, 0, sizeof(buf));
    char tmp[DIRSIZ + 1];
    char *fname1;
    char *p;
    int fd;
    struct dirent de;
    struct stat st;
    fd = open(path, 0);
    if (fd < 0)
    {
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        memset(tmp, 0, sizeof(tmp));
        fname1 = fmtname(path, tmp);
        if (strcmp(fname1, name) == 0)
        {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
        {
            fprintf(2, "ls: path is too long\n");
            close(fd);
            return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        // printf("path = %s\n", buf);
        // 读取目录中的信息
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
            {
                continue;
            }
            // printf("de.name = %s    strcmp = %d  \n", de.name, strcmp(de.name, "."));
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            else
            {
                // printf("strlen(de.name) = %d\n", strlen(de.name));
                memmove(p, de.name, strlen(de.name));
                *(p + strlen(de.name)) = '\0';
                // printf("memmove   path = %s    stat(buf, &st) = %d\n", buf, stat(buf, &st));
                if (stat(buf, &st) < 0)
                {
                    // fprintf(2, "ls: cannot stat %s\n", buf);
                    continue;
                }
                matchFile(buf, name);
            }
        }
        break;
    }
    close(fd);
    return;
}

void ls(char *path, char *name)
{
    matchFile(path, name);
}

int main(int args, char *argv[])
{
    if (args < 2)
    {
        fprintf(2, "输入查找路径和文件名称\n");
        exit(0);
    }
    ls(argv[1], argv[2]);
    exit(0);
}