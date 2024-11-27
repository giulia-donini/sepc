#include "Client/CL_include"
#include "Client/CL_redacteur.h"
#include "Client/CL_driver.h"
#include "Client/CL_moniteur.h"

void redacteur(int voie){
	static int times[2];    // compteur de données accumulées pour chaque voie
	static int buffer1[5];  // buffer pour les données de la voie 1
	static int buffer2[5];  // buffer pour les données de la voie 2

	int* buffer[2];         // tableau des buffers
	buffer[0] = buffer1;
	buffer[1] = buffer2;

    /* lit la donnée depuis le pipe et l’ajoute au buffer correspondant. */
    read(pipe_st[voie][0], &buffer[voie][times[voie]], sizeof(int));
    times[voie]++;
    
    /* si le buffer est plein, envoie les données au driver */
    if (times[voie] == 5) {
        send_to_driver(buffer[voie], 5, voie);
        times[voie] = 0;
    }
}
