#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
    "/bin/bash",
    NULL
};

int child_main(void* args) {
    printf("子进程开始! \n");
    // UTS namespace，主机名和域名隔离
    sethostname("NewNamespace", 12);
    execv(child_args[0], child_args);
    return 1;
}

int main() {
    printf("主程序开始! \n");
    // 等效于 fork
    // flags一共有20多种 CLONE_* 标志：CLONE_NEWIPC, CLONE_NEWNS, CLONE_NEWNET, CLONE_PID, CLONE_NEWUSER, CLONE_NEWUTS 等 
    // UTS(UNIX Time-sharing System) namespace 提供了主机名和域名的隔离
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("主程序退出! \n");
    return 0;
}