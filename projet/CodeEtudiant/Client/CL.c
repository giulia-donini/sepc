#include "Client/CL_include"
#include "Client/CL_moniteur.h"
#include "Client/CL_lecteur.h"

int main(int argc, char *argv[])
{

	if (argc < 2)
    {
      printf("\n!!!! Usage : CL <+/-nombre de donnees>\n");
      exit(0);
   	}

  	int nb_iteration=atoi(argv[1]);

  	if (nb_iteration==0)
    {
      printf("\n!!!! nombre de donnees DIFFERENT de 0\n");
      exit(0);
    }

  signal(SIGUSR1, litbuf1);
  signal(SIGUSR2, litbuf2);
  
	printf("\n* * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("Projet CLIENT - SERVEUR\tTemps Réel E2i - Novembre\n");
	printf("* * * * * * * * * * * * * * * * * * * * * * * * *\n");
	printf("\nLancement du client pendant %d secondes\n", nb_iteration);

  moniteur(nb_iteration);

  return 0;
}	   	   