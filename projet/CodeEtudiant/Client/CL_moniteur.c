#include "Client/CL_include"
#include "Client/CL_moniteur.h"
#include "Client/CL_lecteur.h"
#include "Client/CL_redacteur.h"

int msg_id;

/*     MONITEUR     */	   	   	   
void moniteur(int nb_iteration) {
    pid_t lecteur1_pid, lecteur2_pid;
    pid_t redacteur1_pid, redacteur2_pid;

    // Connecter au serveur et memoire attaché
    connectCL();

    // Créer des processus de lecteur pour les deux canaux
    lecteur1_pid = fork();
    if (lecteur1_pid == 0) {
        lecteur(1); // Processus fil est le Lecteur pour la voie 1
        exit(0);
    }

    lecteur2_pid = fork();
    if (lecteur2_pid == 0) {
        lecteur(2); // Processus fil est le Lecteur pour la voie 2
        exit(0);
    }

    redacteur1_pid = fork();
    if (redacteur1_pid == 0) {
        redacteur(1); 
        exit(0);
    }

    redacteur2_pid = fork();
    if (redacteur2_pid == 0) {
        redacteur(2); 
        exit(0);
    }

    signal(SIGUSR1, litbuf1);
	signal(SIGUSR2, litbuf2);	


    // Boucle principale d'écoute des signaux pendant la durée spécifiée
    for (int i = 0; i < nb_iteration; i++) {
        pause();  // Attendre les signaux SIGUSR1 et SIGUSR2
    }
        
    // Détacher la mémoire partagée avant de quitter
    if (shared_mem_ptr != NULL) {
        shmdt(shared_mem_ptr);
    }

    disconnectCL();

    kill(lecteur1_pid, SIGKILL); // Tuer lecteur1 processus
    kill(lecteur2_pid, SIGKILL); // Tuer lecteur2 processus
    kill(redacteur1_pid, SIGKILL);
    kill(redacteur2_pid, SIGKILL);
}

/*/////////////////////////////////////////////////////////////////////////*/

/*     CONNEXION AU SERVEUR     */	   	   	   
void connectCL(){

    dmsgbuf message;
	key_t cle;

	cle = ftok(CleServeur, C_Msg); // generer la cle pour la messagerie publique

	if (cle == -1) {
        perror("Erreur lors de la génération de la clé");
        exit(EXIT_FAILURE);
    }

	msg_id = msgget(cle, 0666|IPC_CREAT);
    if (msg_id == -1) {	
        perror("Erreur lors de l'accès à la file de messages");
        exit(EXIT_FAILURE);
    }

	message.type = CONNECT;
	pid_t cl_pid = getpid();
	sprintf(message.txt, "%d", cl_pid);  // envoyer le PID du client

	if (msgsnd(msg_id, &message, L_MSG, 0) == -1){
		perror("Erreur envois de message\n");
		exit(EXIT_FAILURE);
	}
	printf("Client (PID %d) : Message CONNECT envoyé.\n", cl_pid);

	if (msgrcv(msg_id, &message, L_MSG, cl_pid, 0) == -1){
		perror("Erreur reception de message\n");
		exit(EXIT_FAILURE);
	}
	printf("Client (PID %d) : Message reçu de type %ld avec contenu '%s'.\n", cl_pid, message.type, message.txt); 

	if (strcmp(message.txt, "") == 0) {
        printf("Le serveur est occupé. Réessayez plus tard.\n");
        exit(EXIT_FAILURE);
    }


    // Convertir le contenu du fichier de la mémoire partagée reçue en clé
    key_t shared_key;
    shared_key = ftok(message.txt, C_Shm);

    printf("Client (PID %d) : Clé d'accès à la mémoire partagée reçue : %d\n", cl_pid, shared_key);

    // Attacher la mémoire partagée
    int shared_mem_id = shmget(0, sizeof(BUF), 0644|IPC_CREAT);
    if (shared_mem_id == -1) {
        perror("Erreur lors de l'accès à la mémoire partagée avec shmget");
        exit(EXIT_FAILURE);
    }

    shared_mem_ptr = (BUF *)shmat(shared_mem_id, NULL, 0);
    if (shared_mem_ptr == (void *)-1) {
        perror("Erreur lors de l'attachement à la mémoire partagée avec shmat");
        exit(EXIT_FAILURE);
    }
    printf("Client (PID %d) : Attaché à la mémoire partagée.\n", cl_pid);


	// Envoyer un acquittement (ACK) au serveur
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
    pid_t cl_pid = getpid();
    message.type = DECONNECT;
    sprintf(message.txt, "%d", cl_pid);

    if (msgsnd(msg_id, &message, L_MSG, 0) == -1){
        perror("Erreur envois de message de déconnexion.\n");
        exit(EXIT_FAILURE);
    }
    printf("Client (PID %d) : Message DECONNECT envoyé.\n", cl_pid);
}
