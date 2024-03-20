#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define RD 0
#define WR 1

void func1(int child_len, char *argv_child[])
{
    char buf[512] = {0};
    // while (read(0, buf, sizeof(buf)))
    // {
    //     printf("pip[RD]  = %s   sizeof = %d \n", buf, strlen(buf));

    //     // memset(buf, 0, sizeof(buf));
    // }
    // if(buf[strlen(buf) - 1] == '\n'){
    //     printf("adada");
    // }

    char *p = buf;
    int len = child_len;

    while (read(0, p, sizeof(char)))
    {
        // printf("--- %s ---\n", buf);
        if (*p == ' ' || *p == '\n')
        {
            int len1 = strlen(buf) - 1;
            // printf("buf = %s\n", buf);
            char tmp[len1 + 1];
            memcpy(tmp, buf, len1);
            *(tmp + len1) = '\0';
            memset(buf, 0, len1);
            argv_child[len++] = tmp;
            for(int i = 0; i < len; ++i){
                printf("   %s   \n", argv_child[i]);
            }
        }
        if (strcmp(p, "\n") == 0)
        {
            // fork()
            argv_child[len] = 0;
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
        if (*p != ' ')
        {
            ++p;
        }
        else
        {
            *p = 0;
            p = buf;
        }
    }
}

// void func(int child_len, char *argv_child[])
// {
//     int pip[2];
//     pipe(pip);

//     // printf("     ---1     ");
//     // for (int i = 0; i < child_len; ++i)
//     // {
//     //     printf("argv_child[%d] = %s\n", i, argv_child[i]);
//     // }

//     int fd = fork();
//     if (fd > 0)
//     {
//         close(pip[RD]);
//         char buf[512] = {0};
//         char *p = buf;
//         while (read(0, p, sizeof(char)))
//         {
//             if (*p == ' ')
//             {
//                 int len1 = strlen(buf);
//                 char tmp[len1 + 1];
//                 memcpy(tmp, buf, len1);
//                 *(tmp + len1) = '\0';
//                 memset(buf, 0, len1 + 1);
//             }
//             else if (*p == '\n')
//             {
//                 break;
//             }
//             ++p;
//         }

//         while (read(0, buf, sizeof(buf)))
//         {
//             printf("pip[RD]  = %s\n", buf);
//             write(pip[WR], buf, sizeof(buf));
//             // char tmp[strlen(buf) + 1];
//             // memcpy(tmp, buf, strlen(buf));
//             // *(tmp + strlen(buf)) = '\0';
//             // argv_child[child_len++] = tmp;
//             memset(buf, 0, sizeof(buf));
//         }

//         // close(0);
//         // dup(pip[WR]);
//         // close(pip[WR]);
//         // close(pip[RD]);
//         // // close(pip[RD]);
//         // printf("     --- 2    ");
//         // dup(pip[WR]);

//         // while (read(pip[RD], buf, sizeof(buf)))
//         // {
//         //     printf("pip[RD]  = %s\n", buf);
//         //     // char tmp[strlen(buf) + 1];
//         //     // memcpy(tmp, buf, strlen(buf));
//         //     // *(tmp + strlen(buf)) = '\0';
//         //     // argv_child[child_len++] = tmp;
//         //     memset(buf, 0, sizeof(buf));
//         // }
//         // printf("     --- 3    ");
//         wait(0);
//         // close(pip[WR]);
//         // close(pip[WR]);
//     }
//     else if (fd == 0)
//     {
//         close(pip[WR]);
//         char buf[512] = {0};
//         // printf("     --- 4    ");
//         while (read(pip[RD], buf, sizeof(buf)))
//         {
//             printf("pip[RD]  = %s\n", buf);

//             char tmp[strlen(buf) + 1];
//             memcpy(tmp, buf, strlen(buf));
//             *(tmp + strlen(buf)) = '\0';
//             argv_child[child_len++] = tmp;
//             memset(buf, 0, sizeof(buf));
//         }
//         argv_child[--child_len] = 0;
//         close(pip[RD]);
//         exec(argv_child[0], argv_child);
//     }
//     else
//     {
//         fprintf(2, "fork error!\n");
//         exit(0);
//     }
// }

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

    // for (int i = 0; i < args; ++i)
    // {
    //     printf("argv_child[%d] = %s\n", i, argv_child[i]);
    // }

    func1(args - 1, argv_child);

    exit(0);
}