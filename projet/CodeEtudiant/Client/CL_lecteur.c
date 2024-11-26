#include "Client/CL_include"
#include "Client/CL_moniteur.h"
#include "Client/CL_lecteur.h"

int lecteur(int voie){
    
    // attacher la mémoire partagée
    int CLTshmid = shmget(shared_key, 2 * sizeof(BUF), 0666 | IPC_CREAT);
    if (CLTshmid == -1) {
        perror("shmget");
        exit(1);
    }
    shared_mem_ptr = (BUF *)shmat(CLTshmid, NULL, 0);
    if (shared_mem_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

	int index = (shared_mem_ptr + voie)->n;
	int data = (shared_mem_ptr + voie)->tampon[index];

	if (write(pipe_st[voie][1], &data, sizeof(int)) < 0) {
        perror("Erreur écriture pipe");
        exit(EXIT_FAILURE);
    }
	return data;
}

/*/////////////////////////////////////////////////////////////////////////*/
	   
void litbuf1() /* semaphore 0 */
{
    voie = 0;

	signal(SIGUSR1,litbuf1);
	printf("Signal recu par le main voie 0\n");

    lecteur(voie);
    redacteur(voie);
}


/*/////////////////////////////////////////////////////////////////////////*/

void litbuf2() /* semaphore 1 */
{
	voie = 1;	

    signal(SIGUSR2,litbuf2);
	printf("Signal recu par le main voie 1\n");

    lecteur(voie);
    redacteur(voie);
}