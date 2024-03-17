#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

int main(int args, char *argv[])
{

    int buf[34] = {0};
    int pipe1[2];
    int pipe2[2];
    int exit_status = 0;
    int create = pipe(pipe1);
    if (create == -1)
    {
        fprintf(2, "管道创建失败！\n");
        exit_status = 1;
        exit(1);
    }
    create = pipe(pipe2);
    if (create == -1)
    {
        fprintf(2, "管道创建失败！\n");
        exit_status = 1;
        exit(1);
    }

    // 添加素数
    for (int i = 2; i < 36; ++i)
    {
        buf[i - 2] = i;
    }
    int fd = fork();
    if (fd > 0)
    {
        close(pipe1[RD]);
        close(pipe2[WR]);
        while (1)
        {
            write(pipe1[WR], buf, sizeof(buf));
            memset(buf, 0, sizeof(buf));
            int num = read(pipe2[RD], buf, sizeof(buf));
            if (num == 0)
            {
                break;
            }
        }
        close(pipe1[WR]);
        close(pipe2[RD]);
        exit(exit_status);
    }
    else if (fd == 0)
    {
        close(pipe1[WR]);
        close(pipe2[RD]);
        while (1)
        {
            read(pipe1[RD], buf, sizeof(buf));
            int prime = buf[0];
            printf("prime %d\n", prime);
            if (buf[1] == 0)
            {
                close(pipe1[RD]);
                close(pipe2[WR]);
                exit(exit_status);
            }
            
            // 数据都在buf中
            int buf1[34] = {0};
            int j = 0;
            for (int i = 1; i < sizeof(buf) / sizeof(int); ++i)
            {
                int tmp = buf[i];
                if (tmp % prime != 0)
                {
                    buf1[j++] = tmp;
                }
            }
            write(pipe2[WR], buf1, sizeof(buf));
            memset(buf, 0, sizeof(buf));
        }
    }
    else
    {
        fprintf(2, "fork() 错误！\n");
        exit(1);
    }

    return exit_status;
}