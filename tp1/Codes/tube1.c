/*
Communication bidirectionnelle avec un seul pipe

On utilisera deux tubes SI : 
la synchronisation est impossible à mettre en oeuvre et que
le père et le fils doivent avoir indifféremment la possibilité de lire et d'écrire à volonté
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
   int tube[2];                     // Tableau extrémités tube        
 
   char chaine[SZ_STRING];             // Chaine stockage message        
   int len;                      // Longueur chaine
   int i;                           // Indice de boucle
 
   // Création tube
   if (pipe(tube) != 0)
   {
      fprintf(stderr, "ligne %u - pipe() - %s\n", __LINE__, strerror(errno));
      exit(errno);
   }
 
   // Création du process fils 
   switch(pid=fork())
   {
      case (-1): // Erreur de fork 
         fprintf(stderr, "ligne %u - fork() - %s\n", __LINE__, strerror(errno)); 
         exit(errno);
 
      case 0: // Fils 
         // Attente que le père ait commencé 
         sleep(1);
 
         // Lecture du tube 
         read(tube[0], chaine, SZ_STRING);
         printf("\tFils(%u) - J'ai reçu '%s'\n", getpid(), chaine);
 
         // On double la chaine (echo)
         len=strlen(chaine);
         for (i=len; i >= 0; i--)
            chaine[i+len]=chaine[i];
 
         // Ecriture dans le tube - N'oublions pas le '\0'
         write(tube[1], chaine, strlen(chaine) + 1);
 
         // Fermeture tube
         close(tube[0]);
         close(tube[1]);
 
         // Arret du fils 
         printf("\tFils(%d) - Je m'arrête\n", getpid());
         exit(0);
 
      default: // Père 
         printf("Je suis le père %u de mon fils %u\n", getpid(), pid);
 
         // Saisie de la chaine 
         printf("Père(%u) - Entrez une chaine (EOT pour sortir) : ", getpid());
         fflush(stdin);
         fgets(chaine, SZ_STRING, stdin);
 
         // Suppression retour charriot et envoi de la chaine
         if (strrchr(chaine, '\n') != NULL)
            *strrchr(chaine, '\n') = 0;
         write(tube[1], chaine, strlen(chaine) + 1);
 
         // Attente que le fils ait traité ma chaine 
         sleep(1);
 
         // Lecture du tube 
         read(tube[0], chaine, SZ_STRING);
         printf("Père(%u) - J'ai reçu '%s'\n", getpid(), chaine);
 
         // Fermeture tube
         close(tube[0]);
         close(tube[1]);
 
         // Attente mort du fils 
         wait(&status);
         printf("Mon fils %u vient de mourir - Mort(%u)\n", pid, WEXITSTATUS(status));
   }
   
   return 0;
}
