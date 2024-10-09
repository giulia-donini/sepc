#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	/* int pid;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(-1);
	}

	// exercice 1
	if (pid == 0) {
		printf("Je suis le fils, mon PID est %d et mon père a le PID %d \n", (int) getpid(), (int) getppid());
	}
	if (pid != 0) {
		printf("Je suis le père, mon PID est %d et j'ai créé un fils dont le PID est %d \n", (int) getpid(), pid);
	} */

	// exercice 2
	if (!fork()){
		printf("Fils 1: je lance gnome calculator! \n");
		char *argv[2];
        argv[0]="gnome-calculator"; // its in /bin/
        argv[1]=NULL;
        execvp(argv[0],argv);
	}

	if (!fork()){
		printf("Fils 2: je lance gnome text editor! \n");
		char *argv[2];
        argv[0]="gnome-text-editor";
        argv[1]=NULL;
		execvp(argv[0],argv);
	}
}

