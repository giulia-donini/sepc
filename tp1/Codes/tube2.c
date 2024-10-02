/*
Communication bidirectionnelle avec deux tubes
*/

#include <sys/wait.h>                  // Attente fin de process         
#include <stdio.h>                     // I/O fichiers classiques        
#include <stdlib.h>                    // Librairie standard
#include <unistd.h>                    // I/O standards (read, write, pipe, fork ...)
#include <string.h>                    // Gestion chaines
#include <signal.h>                    // Signaux de communication       
#include <errno.h>                     // Erreurs système                
 
#define SZ_STRING       (128)       // Taille message                 
 
// Fonction principale 
int main()
{
   // Déclaration des variables 
   int pid;                      // No process                     
   int status;                      // Etat fin fils                  
   int tubePversF[2];                     // Tableau extrémités tube
   int tubeFversP[2];                     // Tableau extrémités tube        
 
   char chaine[SZ_STRING];             // Chaine stockage message        
   int len;                      // Longueur chaine
   int i;                           // Indice de boucle
 
   // Création tube
   if (pipe(tubePversF) != 0)
   {
      fprintf(stderr, "ligne %u - pipe(PversF) - %s\n", __LINE__, strerror(errno));
      exit(errno);
   }
   if (pipe(tubeFversP) != 0)
   {
      fprintf(stderr, "ligne %u - pipe(PversF) - %s\n", __LINE__, strerror(errno));
      exit(errno);
   }
 
   // Création du process fils 
   switch(pid=fork())
   {
      case (-1): // Erreur de fork 
         fprintf(stderr, "ligne %u - fork() - %s\n", __LINE__, strerror(errno)); 
         exit(errno);
 
      case 0: // Fils 
         // Fermeture des extrémités non utilisées
         close(tubePversF[1]);
         close(tubeFversP[0]);
         
         // Lecture du tube 
         read(tubePversF[0], chaine, SZ_STRING);
         printf("\tFils(%u) - J'ai reçu '%s'\n", getpid(), chaine);
 
         // On double la chaine (echo)
         len=strlen(chaine);
         for (i=len; i >= 0; i--)
            chaine[i+len]=chaine[i];
 
         // Ecriture dans le tube - N'oublions pas le '\0'
         write(tubeFversP[1], chaine, strlen(chaine) + 1);
 
         // Fermeture tube
         close(tubePversF[0]);
         close(tubeFversP[1]);
 
         // Arret du fils 
         printf("\tFils(%d) - Je m'arrête\n", getpid());
         exit(0);
 
      default: // Père 
         printf("Je suis le père %u de mon fils %u\n", getpid(), pid);
 
         // Fermeture des extrémités non utilisées
         close(tubePversF[0]);
         close(tubeFversP[1]);
         
         // Saisie de la chaine 
         printf("Père(%u) - Entrez une chaine (EOT pour sortir) : ", getpid());
         fflush(stdin);
         fgets(chaine, SZ_STRING, stdin);
 
         // Suppression retour charriot et envoi de la chaine
         if (strrchr(chaine, '\n') != NULL)
            *strrchr(chaine, '\n') = 0;
         write(tubePversF[1], chaine, strlen(chaine) + 1);
 
         // Lecture du tube 
         read(tubeFversP[0], chaine, SZ_STRING);
         printf("Père(%u) - J'ai reçu '%s'\n", getpid(), chaine);
 
         // Attente mort du fils 
         wait(&status);         
         printf("Mon fils %u vient de mourir - Mort(%u)\n", pid, WEXITSTATUS(status));
         
         // Fermeture tube
         close(tubeFversP[0]);
         close(tubePversF[1]);
   }
   
   return 0;
}
