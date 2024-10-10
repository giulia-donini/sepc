
#include "Client/CL_include"
#include "Client/CL_def.h"
//#include "redacteur.h"
//#include "lecteur.h"


void litbuf1();
void litbuf2();

int voie;			/*variable globale (numero du canal a lire)*/

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


	/////////// CONNECTION AU SERVEUR ////////////

	dmsgbuf message;
	key_t cle;

	cle = ftok(CleServeur, C_Msg); // generer la cle pour la messagerie publique

	if (cle == -1) {
        perror("Erreur lors de la génération de la clé");
        exit(EXIT_FAILURE);
    }

	int msg_id = msgget(cle, 0666|IPC_CREAT);
    if (msg_id == -1) {	
        perror("Erreur lors de l'accès à la file de messages");
        exit(EXIT_FAILURE);
    }

	message.type = CONNECT;
	pid_t cl_pid = getpid();
	sprintf(message.txt, "%d", cl_pid);  // envoyer le PID du client

	if (msgsnd(msg_id, &message, strlen(message.txt), IPC_NOWAIT) == -1){
		perror("Erreur envois de message\n");
		exit(EXIT_FAILURE);
	}

	if (msgrcv(msg_id, &message, strlen(message.txt), message.type, 0) == -1){
		perror("Erreur reception de message\n");
		exit(EXIT_FAILURE);
	}	
	

        /* TO BE CONTINUED */
        

return 0;
}	   	   

	   	   	   
/*/////////////////////////////////////////////////////////////////////////*/
	   
void litbuf1() /* semaphore 0 */
{
	voie=0;
	signal(SIGUSR1,litbuf1);
}


/*/////////////////////////////////////////////////////////////////////////*/

void litbuf2()
{
	voie=1;	
	signal(SIGUSR2,litbuf2);
}
