/***************************
** Fichier    CL_moniteur.h       **
***************************/
#ifndef CL_MONITEUR_H
#define CL_MONITEUR_H

#include "CL_def.h"

/* pipes pour la lecture et écriture des données */
extern int pipe1[2];
extern int pipe2[2];
extern int *pipe_st[2];

extern int voie;			/*variable globale (numero du canal a lire)*/
extern BUF *shared_mem_ptr; /* pointeur vers la mémoire partagée */
extern key_t shared_key;    /* clé de la mémoire partagée */

void connectCL();
void disconnectCL();
void moniteur(int nb_iteration);

#endif /* CL_MONITEUR_H */