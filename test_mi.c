#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[],char *envp[])
{char **ola;
	
pid_t pid;
	//ola=malloc(2);
	argv[0]="echo";
	argv[1]=strdup("ppp");
	argv[2]="ti";
	argv[3]=NULL;
	pid=fork();
	//ola[2]=NULL;
	//argv[1]="ola";
	//execve("/bin/sh",NULL,NULL);
	//execve("/bin/ls",argv,envp);
	
	if(!pid)
		execve("/bin/echo",argv,envp);
	//execve("/bin/pwd",argv,envp);
	wait(&pid);
	//sleep(5);
		printf("o t\n");

return 0;

}
