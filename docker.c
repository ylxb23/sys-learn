/**
 * 手撸一个类似docker的工具
 */

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
    "/bin/bash",
    NULL
};


void printArgs(int argc, char *argv[]) {
    printf("输入参数[%d]: ", argc);
    for(int i=0; i<argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
}

int custom_process(char **args) {
    printf("===============in================");
    int argc = 0;
    
    if(*args != NULL) {

    }
    while(*args != NULL) {
        printf("%s ", *str);
        str++;
        argc++;
    }

    // 重置指针
    str = (char**)args;
    // UTS namespace，主机名和域名隔离
    sethostname("MyDockerNamespace", 17);
    // 开启执行窗口
    // execv(child_args[0], child_args);
    char *cmd = str[0];
    printf("cmd: %s, argc: %d \n", cmd, argc);
    char **argv;
    if(argc > 1) {
        argv = malloc((argc - 1) * sizeof(char*));
        for(int i=1; i<argc; i++) {
            argv[i-1] = strdup(str[i]);
            printf("%s ", argv[i-1]);
        }
    } else {
        argv = NULL;
    }
    printArgs(argc - 1, argv);
    int exec_result = execv(cmd, argv);
    free(argv);
    return exec_result;
}

/**
 * Usage:
 *      ./docker.o <需要在隔离进程中执行的程序> [执行程序的参数]...
 */
int main(int argc, char* argv[]) {
    printArgs(argc, argv);
    if(argc < 2) {
        printf("参数不对, 使用规则: ./docker.o <子程序> [子程序参数]... \n");
        return 1;
    }
    char **inner_argv = malloc((argc - 1) * sizeof(char*));
    for(int i=1; i<argc; i++) {
        inner_argv[i - 1] = strdup(argv[i]);
    }
    printf("cmd:::::: %s %s \n", inner_argv[0], inner_argv[1]);
    // flags一共有20多种 CLONE_* 标志：CLONE_NEWIPC, CLONE_NEWNS, CLONE_NEWNET, CLONE_PID, CLONE_NEWUSER, CLONE_NEWUTS 等 
    // UTS(UNIX Time-sharing System) namespace 提供了主机名和域名的隔离
    // IPC(Inter-Process Communication)进程间通信，涉及的IPC资源包括常见的信号量、消息队列和共享内存。
    int child_pid = clone(custom_process, child_stack + STACK_SIZE, CLONE_NEWIPC| CLONE_NEWUTS | SIGCHLD, inner_argv);
    waitpid(child_pid, NULL, 0);
    free(inner_argv);
    puts("程序退出!");
    return 0;
}

