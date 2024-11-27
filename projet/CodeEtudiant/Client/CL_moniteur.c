#include "Client/CL_include"
#include "Client/CL_moniteur.h"
#include "Client/CL_lecteur.h"
#include "Client/CL_redacteur.h"

/* variables de la messagerie */
int msg_id;
int voie;
BUF *shared_mem_ptr = NULL;
key_t shared_key;

/* pipes pour la lecture et écriture des données */
int pipe1[2];
int pipe2[2];
int* pipe_st[2];

/*     MONITEUR     */	   	   	   
void moniteur(int nb_iteration) {

    /* connecter au serveur */
    connectCL();

	/* création des pipes pour la communication interne entre lecteur et rédacteur */
	pipe(pipe1);
	pipe(pipe2);
	pipe_st[0] = pipe1;
	pipe_st[1] = pipe2;

    signal(SIGUSR1, litbuf1);
	signal(SIGUSR2, litbuf2);

    /* boucle principale d'écoute des signaux pendant la durée spécifiée */
    for (int i = 0; i < nb_iteration; i++) {
        pause();  // attendre les signaux SIGUSR1 et SIGUSR2
    }
        
    /* détacher la mémoire partagée avant de quitter */
    if (shared_mem_ptr != NULL) {
        shmdt(shared_mem_ptr);
    }

    /* déconnexion du serveur */
    disconnectCL();
}

/*/////////////////////////////////////////////////////////////////////////*/

/*     CONNEXION AU SERVEUR     */	   	   	   
void connectCL(){

    dmsgbuf message;

    /* generer la clé pour la messagerie publique */
	key_t cle = ftok(CleServeur, C_Msg);

	if (cle == -1) {
        perror("Erreur lors de la génération de la clé");
        exit(EXIT_FAILURE);
    }

    /* accède à la file de messages */
	msg_id = msgget(cle, 0666|IPC_CREAT);
    if (msg_id == -1) {	
        perror("Erreur lors de l'accès à la file de messages");
        exit(EXIT_FAILURE);
    }

    /* envoie un message de connexion avec le PID du client */
	message.type = CONNECT;
	pid_t cl_pid = getpid();
	sprintf(message.txt, "%d", cl_pid);

	if (msgsnd(msg_id, &message, L_MSG, 0) == -1){
		perror("Erreur envois de message\n");
		exit(EXIT_FAILURE);
	}
	printf("Client (PID %d) : Message CONNECT envoyé.\n", cl_pid);

    /* attente de la réponse du serveur avec la clé pour accéder à la mémoire partagée */
	if (msgrcv(msg_id, &message, L_MSG, cl_pid, 0) == -1){
		perror("Erreur reception de message\n");
		exit(EXIT_FAILURE);
	}
	printf("Client (PID %d) : Message reçu de type %ld avec contenu '%s'.\n", cl_pid, message.type, message.txt); 

    /* vérifie si le serveur est saturé (chaine vide reçue) */
	if (strcmp(message.txt, "") == 0) {
        printf("Le serveur est occupé. Réessayez plus tard.\n");
        exit(EXIT_FAILURE);
    }

    /* convertir le contenu du fichier de la mémoire partagée reçue en clé */
    shared_key = ftok(message.txt, C_Shm);

    printf("Client (PID %d) : Clé d'accès à la mémoire partagée reçue : %d\n", cl_pid, shared_key);

	/* envoyer un acquittement (ACK) au serveur */
    dmsgbuf ack_message;
    ack_message.type = ACK;
    snprintf(ack_message.txt, L_MSG, "%d", cl_pid);

    if (msgsnd(msg_id, &ack_message, strlen(ack_message.txt) + 1, 0) == -1){
        perror("Erreur lors de l'envoi du message ACK avec msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Client (PID %d) : Message ACK envoyé.\n", cl_pid); 
}

/*/////////////////////////////////////////////////////////////////////////*/

/*     DÉCONNEXION AU SERVEUR     */	   	   	   
void disconnectCL(){
    dmsgbuf message;

    /* prépare un message de déconnexion avec le PID du client */
    pid_t cl_pid = getpid();
    message.type = DECONNECT;
    sprintf(message.txt, "%d", cl_pid);

    /* envoie un message de déconnexion au serveur */
    if (msgsnd(msg_id, &message, L_MSG, 0) == -1){
        perror("Erreur envois de message de déconnexion.\n");
        exit(EXIT_FAILURE);
    }
    printf("Client (PID %d) : Message DECONNECT envoyé.\n", cl_pid);
}
