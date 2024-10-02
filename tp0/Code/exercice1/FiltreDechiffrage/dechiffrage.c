#include <stdlib.h>
#include <stdio.h>

int main ( int argc, char ** argv )
{
	char cle;
	char chiffre;
	char clair;
	

	if (argc == 2)
		cle = atoi(argv[1]);
	else
		exit(-1);

	chiffre = getchar();
	clair = chiffre - cle;
	putchar(clair);

    exit( EXIT_SUCCESS );
}
