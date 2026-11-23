
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
int is_running = 1;


static void handle_signal(int signo)
{
    printf("handle signal %d\n", signo);
    if (signo == SIGINT)
    {
        printf("收到SIGINI信号\n");
        release_resource();
        exit(EXIT_SUCCESS);
    }
    else if (signo == SIGTERM)
    {
        printf("收到SIGTERM信号\n");
        release_resource();
        exit(EXIT_SUCCESS);
    }
}


void use_resource()
{
    printf("use resource\n");
}


void release_resource()
{
    printf("release resource\n");
}
int main(int argc, char const *argv[])
{
    // 处理信号函数
    signal(SIGINT,handle_signal);
    signal(SIGTERM,handle_signal);


    /* code */
    use_resource();

    while (is_running)
    {
        /* code */
        printf("running...\n");
        sleep(1);
    }



    release_resource();
    return 0;
}
