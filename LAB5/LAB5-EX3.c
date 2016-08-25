#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (void) {

	int fd[2];

	pipe(fd);

	if (fork() == 0) {
		char *argv[] = {"ls", NULL};
		close(1);
		dup2(fd[1],1);
		execv("/bin/ls",argv);

	}
	else {
		char *argv[] = {"cat", NULL};
		close(0);
		dup2(fd[0],0);
		execv("/bin/cat",argv);
	}	

	return 0;
}
