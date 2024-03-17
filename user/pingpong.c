#include "kernel/types.h"
#include "user/user.h"

#define RD 0 // pipe的read端
#define WR 1 // pipe的write端

int main(int args, char *argv[])
{
    char buf = 'a';

    // 父给子发送消息
    int fd1[2];
    int createpip1 = pipe(fd1);
    if (createpip1 == -1)
    {
        printf("管道创建不成功\n");
        exit(0);
    }
    // 子给父发送消息
    int fd2[2];
    int createpip2 = pipe(fd2);
    if (createpip2 == -1)
    {
        fprintf(2, "管道创建不成功\n");
        exit(1);
    }

    int fd = fork();
    int exit_status = 0;
    if (fd > 0)
    {
        close(fd1[0]);

        if (write(fd1[1], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "parent write() error\n");
            exit_status = 1;
        }

        close(fd2[1]);
        if (read(fd2[0], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "parent read() error\n");
            exit_status = 1;
        }

        printf("%d: received pong\n", getpid());
        close(fd1[1]);
        close(fd2[0]);
        exit(exit_status);
    }
    else if (fd == 0)
    {
        close(fd1[1]);

        if (read(fd1[0], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "child read() error\n");
            exit_status = 1;
        };

        printf("%d: received ping\n", getpid());
        close(fd2[0]);
        if (write(fd2[1], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "child write() error\n");
            exit_status = 1;
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(exit_status);
    }
    else
    {
        fprintf(2, "fork() error\n");
        exit(1);
    }

    return 0;
}