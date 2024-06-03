#include <unistd.h>
#include <stdio.h>

int main() {
	pid_t fpid;	// fpid 表示fork函数的返回值
	fpid = fork();
	if(fpid < 0) {
		printf("error in fork!");
	} else if(fpid == 0) {
		printf("I am child, process id is %d \n", getpid());
	} else {
		printf("I am parent, process id is %d \n", getpid());
	}
	return 0;
}
