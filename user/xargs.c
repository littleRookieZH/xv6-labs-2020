#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"



void func1(int child_len, char *argv_child[])
{
    char *p = 0;
    int j = 0;
    int len = child_len;

    while (read(0, p, sizeof(char)))
    {
        if (argv_child[len] == 0)
        {
            argv_child[len] = malloc(sizeof(char) * MAXARG);
        }
        argv_child[len][j++] = *p;
        if (*p == ' ' || *p == '\n')
        {
            argv_child[len][--j] = '\0';
            j = 0;
            argv_child[++len] = 0;
        }
        if (*p == '\n')
        {
            if (fork() == 0)
            {
                exec(argv_child[0], argv_child);
            }
            else
            {
                wait(0);
                len = child_len;
            }
        }
    }
}

int main(int args, char *argv[])
{
    if (args < 3)
    {
        fprintf(2, "输入有错\n");
        exit(0);
    }

    char *argv_child[MAXARG];
    for (int i = 1; i < args + 1; ++i)
    {
        argv_child[i - 1] = argv[i];
    }

    argv_child[args] = 0;
    func1(args - 1, argv_child);

    exit(0);
}