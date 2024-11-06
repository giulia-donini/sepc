/***************************
** Fichier    CL_moniteur.h       **
***************************/
#ifndef CL_MONITEUR_H
#define CL_MONITEUR_H

#include "CL_def.h"

extern int voie;			/*variable globale (numero du canal a lire)*/
extern BUF *shared_mem_ptr; /* Pointeur vers la mémoire partagée */

void connectCL();
void disconnectCL();
void moniteur(int nb_iteration);

#endif /* CL_MONITEUR_H */