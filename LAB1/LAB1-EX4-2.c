#include <unistd.h>
#include <sys/wait.h>

int main () {

	int filho_pid;
	char *argv[] = { "echo", "Hello World", NULL };

	filho_pid = fork();
	
	if (filho_pid != 0 ) {
		int status;
		waitpid(filho_pid,&status,0);
	}
	else {
		execv("/bin/echo",argv);
	}
	return 0;
}